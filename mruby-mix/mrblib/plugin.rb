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

  Autoloader = []
end

Plugin.vm.Plugin = Plugin

Mix::Miquire.loadpath << Plugin::Autoloader
