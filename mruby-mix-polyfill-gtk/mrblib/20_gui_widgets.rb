module Plugin::GUI
  # 仕様参考: https://mikutter.hatenablog.com/entry/Postbox%E3%82%92%E4%BD%BF%E3%81%86
  class Postbox
    include Widget
    include CuscadableLite
    include Ephemeral

    attr_accessor :options

    def initialize(name)
      super
      Plugin.call(:postbox_created, self)
    end
  end

  class Timeline
    include Widget
    include CuscadableLite
    include Ephemeral

    def initialize(name)
      super
      Plugin.call(:timeline_created, self)
    end

    def create_reply_postbox(in_reply_to_message, options = {})
      create_postbox(options.merge(to: [in_reply_to_message],
                                   header: "@#{in_reply_to_message.user.idname} "))
    end

    def create_postbox(options = {})
      Plugin::GUI::Postbox.instance.options = options
    end
  end
end
