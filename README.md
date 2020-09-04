mruby-mix
===

Embeddable mikutter runtime

[yukari-exvoice](https://github.com/shibafu528/yukari-exvoice)のコードを、組み込み先を問わず再利用可能な形でmrbgem化するプロジェクト。

このリポジトリには[mikutter](https://mikutter.hachune.net)の成果物([license](https://github.com/mikutter/mikutter/blob/master/LICENSE))と、yukari-exvoiceの成果物([license](https://github.com/shibafu528/yukari-exvoice/blob/master/LICENSE))を基にしたコードが含まれます。

## gems

### mruby-mix

mruby-mixのコアモジュール。mikutterのcoreに含まれるコードに相当。

### mruby-mix-miquire-fs

Miquire プラグインローダーをファイルシステムに対応させるためのコード。

### mruby-mix-twitter-models

mikutter 3.4くらいの頃のTwitter機能で使われていたModelのスタブ。

当時はまだTwitter Pluginとして完全分離されていなかった。

### mruby-mix-polyfill-gtk

GUIおよびGTKプラグインのスタブ。command DSLを使うプラグインが期待するデータ構造の一部が含まれます。

## mixを組み込む開発者の方へ

`docs/` ディレクトリに、プラグインが期待する要件を満たすための手引きをまとめています。ビルド環境が整ったら、こちらをご一読ください。

前提として、mikutterのプラグイン開発に対する一定の知識を要します。  
ドキュメントしきれていない部分を含めて完璧に実装するには、mikutterの内部実装についての知識も必要となるでしょう。
