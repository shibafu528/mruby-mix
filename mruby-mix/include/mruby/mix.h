#ifndef MRUBY_MIX_H
#define MRUBY_MIX_H

#include <mruby.h>

/** mix_register_remain_handler() で使えるコールバック関数の仕様。 */
typedef void (*mix_remain_handler)(mrb_state *mrb);

/**
 * mix_register_reserve_handler() で使えるコールバック関数の仕様。
 * @param delay イベントが何秒後に予約されたかの値。コールバックされた時点からこの時間が経過したら mix_run() が呼ばれることを期待している。
 */
typedef void (*mix_reserve_handler)(mrb_state *mrb, mrb_float delay);

typedef void (*mix_log_handler)(const char *msg);

/** キューされているイベントを全て処理する。 */
void MRB_API mix_run(mrb_state *mrb);

/**
 * イベントがキューされた際に呼び出されるコールバックを登録する。
 * ホスト側で持っているイベントループにキューするために使える。
 */
void MRB_API mix_register_remain_handler(mrb_state *mrb, mix_remain_handler handler);

/**
 * ある時間が経過した後に実行されることを期待したイベントがキューされた際に呼び出されるコールバックを登録する。
 * mix_register_remain_handler() と同様の目的で使える。
 */
void MRB_API mix_register_reserve_handler(mrb_state *mrb, mix_reserve_handler handler);

/** utils.rb経由で出力されたログを受け取るコールバックを登録する。 */
void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler);

/**
 * Kernel.requireを呼び出す。
 * この関数を使う場合はmruby-requireを依存関係に加えること。
 * @param path 読み込むファイルのフルパス
 */
int MRB_API mix_require(mrb_state *mrb, const char *path);

#define mix_class_get(mrb, ...) mix_class_dig(mrb, sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*), (const char*[]){ __VA_ARGS__ })

#define mix_module_get(mrb, ...) mix_module_dig(mrb, sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*), (const char*[]){ __VA_ARGS__ })

/**
 * ネストした空間に対するmrb_class_getの簡易版。
 */
MRB_API struct RClass* mix_class_dig(mrb_state *mrb, size_t count, const char **names);

/**
 * ネストした空間に対するmrb_module_getの簡易版。
 */
MRB_API struct RClass* mix_module_dig(mrb_state *mrb, size_t count, const char **names);

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

/**
 * Pluginのインスタンスを取得、または新規作成します。
 * @param slug プラグインのslug
 */
mrb_value mix_plugin_get(mrb_state *mrb, const char *slug);

/** mix_plugin_get() のエイリアス */
#define mix_plugin_create(mrb, slug) mix_plugin_get(mrb, slug)

/**
 * 現在実行中のイベントリスナーが登録されているPluginを取得する。
 * イベントリスナー関数の外で呼び出してはいけない。
 * @return Plugin
 */
MRB_API mrb_value mix_plugin_event_get_receiver_from_env(mrb_state *mrb);

/**
 * 現在実行中のイベントの名前を取得する。
 * イベントリスナー関数の外で呼び出してはいけない。
 * @return Symbol
 */
MRB_API mrb_value mix_plugin_event_get_name_from_env(mrb_state *mrb);

/**
 * Pluginにネイティブのイベントリスナーを登録する。
 * @param plugin Plugin
 * @param event_name イベント名
 * @param callback コールバック
 * @return Pluggaloid::Listener
 */
MRB_API mrb_value mix_plugin_add_event_listener(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_func_t callback);

/**
 * Pluginにネイティブのイベントフィルタを登録する。
 * @param plugin Plugin
 * @param event_name イベント名
 * @param callback コールバック
 * @return Pluggaloid::Filter
 */
MRB_API mrb_value mix_plugin_add_event_filter(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_func_t callback);

#endif
