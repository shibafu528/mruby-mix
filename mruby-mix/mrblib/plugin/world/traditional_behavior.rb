module Plugin::World
  # レガシーなmikutter サードパーティプラグインが、Worldインスタンスに対して暗黙に期待する振る舞いの定義
  # これをmix-inすることで、レガシープラグインとの一定の互換性を確保できる。
  #
  # レガシープラグインは、当時のServiceクラスのインスタンスメソッドの振る舞いを期待している。
  # 特にmikutter 3.6(2017-12-25)より前の時代に作成されたプラグインに多い。
  # mikutter 4.1現在ではWorldとSpellを組み合わせて使うことで置き換えられている。
  module TraditionalBehavior
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

    # @return [String]
    def idname
      # いい感じに実装しておいてください
      _traditional_idname
    end
    alias user idname
  end
end
