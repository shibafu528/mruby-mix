mruby-mix
===

Embeddable mikutter runtime

[yukari-exvoice](https://github.com/shibafu528/yukari-exvoice)のコードを、組み込み先を問わず再利用可能な形でmrbgem化するプロジェクト。

## gems

### mruby-mix

mruby-mixのコアモジュール。mikutterのcoreに含まれるコードに相当。

### mruby-mix-twitter-models

mikutter 3.4くらいの頃のTwitter機能で使われていたModelのスタブ。

当時はまだTwitter Pluginとして完全分離されていなかった。

### mruby-mix-polyfill-gtk

GUIおよびGTKプラグインのスタブ。