# settings DSL support

class Plugin
  module SettingsDSL
    def self.included(klass)
      klass.extend Methods
    end

    module Methods
      def settings(name, &place)
        warn "DSL settings(#{name}) called, but it doesn't support yet."
      end
    end

    include Methods
  end
end

Plugin.include Plugin::SettingsDSL
