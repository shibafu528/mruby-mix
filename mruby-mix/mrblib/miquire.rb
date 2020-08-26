# core/miquire_plugin.rb の移植

module Mix::Miquire
  class << self
    include Enumerable

    # プラグインのロードパス。ファイルシステム上であれば単にパスを追加すればOK。
    # ファイルシステム以外を使う場合は #each で Mix::Miquire::Plugin を列挙可能なオブジェクトとしてラップしてからここに登録する。
    #
    # @example
    #   Mix::Miquire.loadpath << '/foo/bar' << '/hoge/piyo'
    #   Mix::Miquire.loadpath << AcmePluginCollection.new
    def loadpath
      @loadpath ||= []
    end

    def trees
      loadpath.map(&method(:path_to_tree))
    end

    def path_to_tree(path)
      if path.respond_to?(:each)
        path
      else
        FileSystemTree.new(path)
      end
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

    def load_all
      each(&:load)
    end
  end

  # ファイルシステム上にあるプラグインが格納されたディレクトリ
  class FileSystemTree
    def initialize(path)
      @path = path
    end
    
    def each(&block)
      Dir.foreach(@path) do |entry|
        next if entry == '.' || entry == '..'
        if FileTest.directory?(File.join(@path, entry)) && FileTest.exist?(File.join(@path, entry, "#{entry}.rb"))
          block.(build_structured_plugin(entry))
        elsif not entry.end_with?('.rb')
          next
        else
          block.(build_single_file_plugin(entry))
        end
      end
    end

    def build_structured_plugin(dirname)
      plugin_dir = File.join(@path, dirname)

      spec_filename = File.join(plugin_dir, '.mikutter.yml')
      deprecated_spec = false
      unless FileTest.exist?(spec_filename)
        spec_filename = File.join(plugin_dir, 'spec')
        deprecated_spec = true
      end

      spec = if FileTest.exist?(spec_filename)
               YAML.load(IO.read(spec_filename)).symbolize.merge(deprecated_spec: deprecated_spec)
             else
               { slug: dirname.to_sym, deprecated_spec: false }
             end
      loader = -> do
        Kernel.load(File.join(plugin_dir, "#{spec[:slug]}.rb"))
      end

      Plugin.new(dirname, Spec.new(spec), loader)
    end

    def build_single_file_plugin(filename)
      slug = filename.delete_suffix('.rb').to_sym
      spec = Spec.new(slug: slug, deprecated_spec: false)
      loader = -> do
        Kernel.load(File.join(@path, filename))
      end
      Plugin.new(slug, spec, loader)
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
        begin
          raise LoadError, "plugin #{slug.inspect} was not loaded because dependent plugin #{depend.inspect} was not loaded." unless depend.load
        rescue LoadError => e
          raise LoadError, "plugin #{slug.inspect} was not loaded because dependent plugin was not loaded. previous error is:\n#{e.to_s}"
        end
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
          slug = Array(s).first.to_sym
          if slug
            Mix::Miquire.find_by_slug(slug)
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
