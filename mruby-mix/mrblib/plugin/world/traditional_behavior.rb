module Plugin::World
  # レガシーなmikutter サードパーティプラグインが、Modelに対して暗黙に期待する振る舞いの定義
  # Modelの役割に応じてmix-inすることで、レガシープラグインとの一定の互換性を確保できる。
  module TraditionalBehavior
    # mikutter 3.5 Service (Twitter::World) 互換
    module World
      def post(to: nil, message:, **kwrest)
        Plugin[:world].compose(self, to, body: message, **kwrest)
      end
      alias update post # 厳密にはupdateはmikutwitterの呼び出しだが...

      def service
        self
      end

      # @return [Diva::Model]
      def user_obj
        # いい感じに実装しておいてください
        _traditional_user
      end
      alias to_user user_obj

      def idname
        user_obj.idname
      end
      alias user idname
    end

    # Twitter::Message 互換
    module Message
      # @param [Hash] other
      def post(message:, **kwrest)
        world, = Plugin[:world].filtering(:current_world, nil)
        Plugin[:world].compose(world, self, body: message, **kwrest)
      end

      def message
        self
      end
    end
  end
end
