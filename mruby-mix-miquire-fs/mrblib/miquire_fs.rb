module Mix::Miquire
  class << self
    alias path_to_tree_original path_to_tree
    def path_to_tree(path)
      path_to_tree_original(path) || FileSystemTree.new(path)
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
end
