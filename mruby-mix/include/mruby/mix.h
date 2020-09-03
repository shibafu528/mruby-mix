#ifndef MRUBY_MIX_H
#define MRUBY_MIX_H

#include <mruby.h>

typedef void (*mix_remain_handler)(mrb_state *mrb);
typedef void (*mix_log_handler)(const char *msg);

/** キューされているイベントを全て処理する。 */
void MRB_API mix_run(mrb_state *mrb);

/**
 * イベントがキューされた際に呼び出されるコールバックを登録する。
 * ホスト側で持っているイベントループにキューするために使える。
 */
void MRB_API mix_register_remain_handler(mrb_state *mrb, mix_remain_handler handler);

/** utils.rb経由で出力されたログを受け取るコールバックを登録する。 */
void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler);

/**
 * Kernel.requireを呼び出す。
 * この関数を使う場合はmruby-requireを依存関係に加えること。
 * @param path 読み込むファイルのフルパス
 */
int MRB_API mix_require(mrb_state *mrb, const char *path);

/**
 * commandのProc呼出引数として使うPlugin::GUI::Eventを生成する。
 * この関数を使うにはmruby-mix-polyfill-gtkが必要。
 * @param event コマンドイベントの発生元。mikutterのメニューコマンドおよびショートカットコマンドでは contextmenu が使用される。
 * @param widget コマンドイベントが発生したWidget。例えば、Plugin::GUI::TimelineやPlugin::GUI::Postboxのインスタンスを渡す。
 * @param messages 関連するMessageを格納したArray。Messageとしての振る舞いを持ったDiva::Modelとして渡す必要がある。
 * @param world このイベントを処理する必要のあるWorld。明示的な指定が必要なければ、単にカレントのWorldを渡す。
 * @return Plugin::GUI::Event
 */
mrb_value MRB_API mix_gui_event_new(mrb_state *mrb, const char *event, mrb_value widget, mrb_value messages, mrb_value world);

#endif
