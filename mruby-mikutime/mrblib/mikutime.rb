# -*- coding: utf-8 -*-

Delayer.default = Delayer.generate_class(priority: [:high, :normal, :low], default: :normal)

# 歌うだけならきっと誰でもできる
# わたしはきっとそこらにいない女の子
module Mikutime
  class DoubleInitError < RuntimeError; end
  
  class << self
    # 現在の環境にmikutter runtimeを展開する。
    # このメソッドは1回だけ呼びだす。
    def init!
      raise DoubleInitError if @@initialized
      @@initialized = true
    end

    # み・く・た・い・む! みくたいむ! 起動できました! 発射しまーす!!
    def boot!
      Plugin.call :boot
    end
  end
end
