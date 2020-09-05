# command DSL support

Plugin.autoload(:gui) do
  defdsl :command do |slug, options, &exec|
    command = options.merge(slug: slug, exec: exec, plugin: @name).freeze
    add_event_filter(:command) { |menu|
      menu[slug] = command
      [menu]
    }
  end
end
