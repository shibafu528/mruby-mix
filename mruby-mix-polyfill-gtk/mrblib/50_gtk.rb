# encoding: utf-8
# core/plugin/gtk 互換プラグイン

module Plugin::Gtk
  class GtkError < StandardError; end

  class Postbox
    TextView = Struct.new(:buffer)
    Buffer = Struct.new(:text)

    attr_reader :widget_post
    attr_accessor :options

    def initialize(**options)
      @options = options
      @widget_post = TextView.new(Buffer.new(''))
    end
  end
end

Plugin.autoload(:gtk) do
  # 互換クラスのインスタンスを保持する
  @instances = {
    Plugin::GUI::Postbox => Hash.new
  }

  def widgetof(gui_instance)
    @instances[gui_instance.class][gui_instance.slug]
  end

  on_postbox_created do |i_postbox|
    options = i_postbox.options || Hash.new
    @instances[Plugin::GUI::Postbox][i_postbox.slug] = Plugin::Gtk::Postbox.new(**options)
  end

  on_gui_destroy do |i_widget|
    if store = @instances[i_widget.class]
      store.delete(i_widget.slug)
    end
  end
end
