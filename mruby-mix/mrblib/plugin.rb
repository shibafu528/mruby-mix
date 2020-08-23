# mikutter-compatible Plugin constants

Event = Pluggaloid::Event
EventListener = Pluggaloid::Listener
EventFilter = Pluggaloid::Filter

class Plugin < Pluggaloid::Plugin; end

Plugin.vm.Plugin = Plugin
