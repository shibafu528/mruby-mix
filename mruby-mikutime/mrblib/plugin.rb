Event = Pluggaloid::Event
EventListener = Pluggaloid::Listener
EventFilter = Pluggaloid::Filter

class Plugin < Pluggaloid::Plugin

  def command(slug, options, &exec)
    command = options.merge(slug: slug, exec: exec, plugin: @name).freeze
    add_event_filter(:command) { |menu|
      menu[slug] = command
      [menu]
    }
  end
  
end

Plugin.vm.Plugin = Plugin
