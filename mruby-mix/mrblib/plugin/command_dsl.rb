# command DSL support

class Plugin
  module CommandDSL
    def command(slug, options, &exec)
      command = options.merge(slug: slug, exec: exec, plugin: @name).freeze
      add_event_filter(:command) { |menu|
        menu[slug] = command
        [menu]
      }
    end
  end
end

Plugin.include Plugin::CommandDSL
