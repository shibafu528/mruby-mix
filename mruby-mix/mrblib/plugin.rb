# mikutter-compatible Plugin constants

Event = Pluggaloid::Event
EventListener = Pluggaloid::Listener
EventFilter = Pluggaloid::Filter

class Plugin < Pluggaloid::Plugin
  class << self
    def autoload(slug, &block)
      Autoloader << Mix::Miquire::Plugin.new(slug, Mix::Miquire::Spec.new(slug: slug), -> { Plugin.create(slug, &block) })
      slug
    end
  end

  # command DSLの実体はmruby-mix-polyfill-gtk内のgui pluginにある。
  # このメソッドではgui pluginのロードを試みた上で転送する。
  def command(slug, options, &exec)
    if Plugin.instance_exist?(:gui)
      raise NoMethodError, "undefined method 'command' for #{self}"
    end

    gui = Mix::Miquire.find_by_slug(:gui)
    unless gui
      raise NoMethodError, "undefined method 'command' for #{self}"
    end

    gui.load
    command(slug, options, &exec)
  end

  Autoloader = []
end

Plugin.vm.Plugin = Plugin

Mix::Miquire.loadpath << Plugin::Autoloader
