# activity DSL support

class Plugin
  module ActivityDSL
    def self.included(klass)
      klass.extend Methods
    end

    module Methods
      def activity(kind, title, args = {})
        Plugin.call(:modify_activity,
                    { plugin: nil,
                      kind: kind,
                      title: title,
                      date: Time.new,
                      description: title }.merge(args))
      end
    end

    include Methods
  end
end

Plugin.include Plugin::ActivityDSL
