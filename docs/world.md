World について
====

mikutter における `World` プラグインは、マルチサービス・マルチアカウントを管理するための基盤となっているものです。  
詳細については下記を参照してください。

* https://dev.mikutter.hachune.net/issues/989
* https://mikutter.hatenablog.com/entry/2017/12/07/200000
* https://mikutter.hatenablog.com/entry/oshaburi0

一方 mruby-mix における `World` プラグインは、ホスト アプリケーションが持つアカウント情報を一方的にmrubyコードに伝達するためのプロキシとしての役割のみを持ちます。  
mikutterでは可能だったアカウントの永続化のような操作は、今のところできないようになっています。

## ホスト アプリケーションの要件

ホスト アプリケーションからアカウント情報を通知するためには、最低でも以下の2つを実装する必要があります。

* Worldインスタンスを表すためのDiva Model
* Filter `:world_current(world)`
  * ホスト側でこのフィルタを実装し、カレントアカウントを通知できるようにする

使用可能なアカウントのリストを認識させるためには、以下についても実装してください。

* `Plugin::World::Gate.accounts` メソッド

追加で、以下の機能も実装することができます。

* Event `:world_create(world)`
  * Worldがホスト側で追加された際に、mruby側に同期する
* Event `:world_destroy(world)`
  * Worldがホスト側で削除された際に、mruby側に同期する
* Event `:world_change_current(world)`
  * mruby側からのカレントアカウント変更要求を受けられるようにする

## `Plugin::World::Gate.accounts`

```ruby
module Plugin::World::Gate
  # @return [Array<Diva::Model>]
  def self.accounts
  end
end
```

```c
struct RClass *mod_plugin = mrb_define_module(mrb, "Plugin");
struct RClass *mod_world = mrb_define_module_under(mrb, mod_plugin, "World");
struct RClass *mod_gate = mrb_define_module_under(mrb, mod_world, "Gate");
mrb_define_module_function(mrb, mod_gate, "accounts", YOUR_FUNCTION, MRB_ARGS_NONE());
```

mrubyコードからホスト アプリケーションに対して、すべてのアカウント情報を要求するために呼び出されるメソッドです。  
ホスト側ではこのメソッドを定義して、呼び出された際はmruby側に公開したいアカウント情報をDiva Modelに変換した後戻り値とする必要があります。

## Diva Modelの要件

Diva Modelとしてのインターフェースは、一般的なWorld提供プラグイン相当のものであれば差し支えありません。  
Spell等によって処理がホスト側に戻ってきた時に備えて、このModel内にホスト側でアカウントを特定するための識別情報やポインタを持たせておいても構いません。

念の為、ライフサイクルがホストとmrubyで正確に一致するとは限らないので、必要でなければポインタを持ち回るよりは単純な識別用のキー値のほうが良いでしょう。  
mrubyコードからポインタを隠蔽するテクニックについては `MRB_TT_DATA` でググると情報が得られます。

なお、ここで作成したDiva Modelのポインタは、GC Protectを行った上でホスト側で適切に保持することを推奨します。  
参照を失うと `:world_destroy` イベントを呼び出すための手段も失います。
