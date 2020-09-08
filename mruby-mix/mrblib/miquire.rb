# core/miquire_plugin.rb の移植

module Mix::Miquire
  class << self
    include Enumerable

    # プラグインのロードパス。#each で Mix::Miquire::Plugin を列挙可能なオブジェクトとしてラップしてからここに登録する。
    # mruby-mix-miquire-fs を追加で導入している場合は、単にディレクトリのパスを登録することも可能。
    #
    # @example
    #   Mix::Miquire.loadpath << AcmePluginCollection.new
    #   Mix::Miquire.loadpath << '/foo/bar' << '/hoge/piyo'
    def loadpath
      @loadpath ||= []
    end

    def trees
      loadpath.map(&method(:path_to_tree))
    end

    # mruby-mix-miquire-fsでのhook用
    def path_to_tree(path)
      path.respond_to?(:each) && path
    end

    def each(&block)
      iterated = Set.new
      detected = []
      trees.reverse.each do |tree|
        tree.each do |plugin|
          if not iterated.include?(plugin.name)
            iterated << plugin.name
            detected << plugin
          end
        end
      end
      detected.each(&block)
    end

    def to_hash
      result = {}
      each do |plugin|
        result[plugin.slug] = plugin
      end
      result
    end

    def find_by_slug(slug)
      to_hash[slug.to_sym]
    end

    def load_all(failfast: false)
      success = []
      failed = []

      each do |plugin|
        plugin.load
        success << plugin.name
      rescue LoadError => e
        raise e if failfast # なぜかRuntimeErrorになるので、raiseと書けない
        error e
        failed << plugin.name
      end

      [success, failed]
    end
  end

  # プラグインのメタデータと環境依存の読み込み処理をまとめたもの
  class Plugin
    def initialize(name, spec, loader)
      @name = name
      @spec = spec
      @loader = loader
    end

    def name
      @name
    end

    def slug
      @spec && @spec[:slug]
    end

    def spec
      @spec
    end

    def load
      return true if ::Plugin.instance_exist?(slug)
      
      spec.depended_plugins.each do |depend|
        depend.load
      rescue LoadError => e
        raise LoadError, "plugin #{slug.inspect} was not loaded because dependent plugin was not loaded. previous error is:\n#{e.to_s}"
      end

      notice "plugin loaded: #{slug}"
      s = spec
      ::Plugin.create(slug) do
        self.spec = s.to_hash
      end
      @loader.()
      
      true
    end
  end

  class MissingPlugin < Plugin
    def initialize(slug)
      super(slug, nil, nil)
    end

    def load
      raise LoadError, "dependent plugin #{name.inspect} was not found."
    end
  end

  # プラグインのメタデータをラップしたもの
  class Spec
    def initialize(attributes = {})
      @attributes = attributes

      # normalize slug
      slug = @attributes[:slug]
      unless slug.is_a? Symbol
        @attributes[:slug] = slug&.delete_prefix(':')&.to_sym
      end
    end

    def [](key)
      @attributes[key]
    end

    # 依存しているプラグインを列挙する
    # @return [Array<Mix::Miquire::Plugin>]
    def depended_plugins
      depends = @attributes.dig(:depends, :plugin)
      if depends
        Array(depends).map do |s|
          notice "finding #{s.inspect}"
          slug = Array(s).first.to_sym
          if slug
            Mix::Miquire.find_by_slug(slug) || MissingPlugin.new(slug)
          else
            slug
          end
        end
      else
        []
      end
    end

    def inspect
      @attributes.inspect
    end

    def to_hash
      @attributes
    end
  end

  class LoadError < StandardError; end
end
