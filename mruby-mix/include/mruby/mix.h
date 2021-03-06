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

/**
 * mix_plugin_event_env() で取得可能な情報の識別子。
 */
typedef enum {
  /** 実行中のイベントリスナーが登録されているPlugin */
  MIX_EVENT_ENV_RECEIVER = 0,
  /** 実行中のイベントの名前 (Symbol) */
  MIX_EVENT_ENV_NAME = 1,
  /** @private */
  MIX_EVENT_ENV_LENGTH
} mix_event_env_t;

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
MRB_API mrb_value mix_plugin_get(mrb_state *mrb, const char *slug);

/** mix_plugin_get() のエイリアス */
#define mix_plugin_create(mrb, slug) mix_plugin_get(mrb, slug)

/**
 * 引数なしでイベントを発生させる。
 * @param event_name イベント名
 * @return Delayer
 */
MRB_API mrb_value mix_plugin_call_arg0(mrb_state *mrb, const char *event_name);

/**
 * 1つの引数を持ったイベントを発生させる。
 * @param event_name イベント名
 * @param arg 最初の引数
 * @return Delayer
 */
MRB_API mrb_value mix_plugin_call_arg1(mrb_state *mrb, const char *event_name, mrb_value arg);

/**
 * 任意数の引数を持ったイベントを発生させる。
 * @param event_name イベント名
 * @param argc 引数の数
 * @param argv 引数の配列
 * @return Delayer
 */
MRB_API mrb_value mix_plugin_call_argv(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv);

/**
 * 1つの引数をフィルタリングした結果を返す。
 * @param event_name イベント名
 * @param arg 最初の引数
 * @return フィルタされた引数
 */
MRB_API mrb_value mix_plugin_filtering_arg1(mrb_state *mrb, const char *event_name, mrb_value arg);

/**
 * 任意数の引数をフィルタリングした結果を返す。
 * @param event_name イベント名
 * @param argc 引数の数
 * @param argv 引数の配列
 * @return フィルタされた引数の配列
 */
MRB_API mrb_value mix_plugin_filtering_argv(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv);

/**
 * 任意数の引数をフィルタリングし、結果の `nth_result` 番目を取り出す。
 * @param event_name イベント名
 * @param argc 引数の数
 * @param argv 引数の配列
 * @param nth_result フィルタ結果から取り出す要素の序数
 * @return フィルタされた引数の `nth_result` 番目の値
 */
MRB_API mrb_value mix_plugin_filtering_and_nth(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv, mrb_int nth_result);

/**
 * 現在実行中のイベントに関する変数(イベント名など)を取得する。
 * イベントリスナー関数の外で呼び出してはいけない。
 * @param item 取得する情報
 */
MRB_API mrb_value mix_plugin_event_env(mrb_state *mrb, mix_event_env_t item);

/**
 * 現在実行中のイベントリスナーが登録されているPluginを取得する。
 * イベントリスナー関数の外で呼び出してはいけない。
 * @return Plugin
 * @deprecated mix_plugin_event_env() を使うこと。
 */
__attribute__((deprecated))
MRB_API mrb_value mix_plugin_event_get_receiver_from_env(mrb_state *mrb);

/**
 * 現在実行中のイベントの名前を取得する。
 * イベントリスナー関数の外で呼び出してはいけない。
 * @return Symbol
 * @deprecated mix_plugin_event_env() を使うこと。
 */
__attribute__((deprecated))
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
 * Pluginにイベントリスナーを登録する。
 * @param plugin Plugin
 * @param event_name イベント名
 * @param proc コールバック (Proc)
 * @return Pluggaloid::Listener
 */
MRB_API mrb_value mix_plugin_add_event_listener_proc(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_value proc);

/**
 * Pluginにネイティブのイベントフィルタを登録する。
 * @param plugin Plugin
 * @param event_name イベント名
 * @param callback コールバック
 * @return Pluggaloid::Filter
 */
MRB_API mrb_value mix_plugin_add_event_filter(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_func_t callback);

/**
 * Pluginにイベントフィルタを登録する。
 * @param plugin Plugin
 * @param event_name イベント名
 * @param proc コールバック (Proc)
 * @return Pluggaloid::Filter
 */
MRB_API mrb_value mix_plugin_add_event_filter_proc(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_value proc);

/**
 * Spellを呼び出す。
 * @param spell_name Spellの名前
 * @param argc 引数の数
 * @param argv 引数の配列
 * @return Delayer::Deferred
 */
MRB_API mrb_value mix_spell(mrb_state *mrb, const char *spell_name, mrb_int argc, const mrb_value *argv);

/**
 * 指定した引数に応答できるSpellが定義されているかを確認する。
 * @param spell_name Spellの名前
 * @param argc 引数の数
 * @param argv 引数の配列
 * @return 呼び出し可能であればTRUE
 */
MRB_API mrb_bool mix_spell_defined(mrb_state *mrb, const char *spell_name, mrb_int argc, const mrb_value *argv);

/**
 * Spellを定義する。
 * @param spell_name Spellの名前
 * @param slug_count 引数となるModelのslugの数
 * @param slugs 引数となるModelのslugの配列
 * @param condition 前提条件のProc。不要な場合は mrb_nil_value() で省略可能。
 * @param body 呼び出された際に実行するProc。
 * @see https://reference.mikutter.hachune.net/basis/2017/12/11/spell.html
 * @return 定義に成功した場合はtrue
 */
MRB_API mrb_value mix_define_spell(mrb_state *mrb, const char *spell_name, mrb_int slug_count, const mrb_sym *slugs, mrb_value condition, mrb_value body);

/**
 * Mix::Miquire.load_all を呼び出す。
 * @return 読み込んだプラグインのslugの配列と読み込みに失敗したプラグインのslugの配列。 例: `[[:foo, :bar, ...], [:baz, ...]]`
 */
MRB_API mrb_value mix_miquire_load_all(mrb_state *mrb);

/**
 * Mix::Miquire.load_all を呼び出す。ただし、1つでも読み込みに失敗したら例外をスローする。
 * @return mix_miquire_load_all() と同じ。ただし、読み込みに失敗した側の配列は常に空となる。
 */
MRB_API mrb_value mix_miquire_load_all_failfast(mrb_state *mrb);

/**
 * Mix::Miquire.load_path の末尾にエントリを追加する。
 * @param loadpath 追加するエントリ
 */
MRB_API void mix_miquire_append_loadpath(mrb_state *mrb, mrb_value loadpath);

/**
 * Mix::Miquire.load_path の末尾に文字列のエントリを追加する。
 * この関数を使ってファイルシステム上のプラグインを読み込むためには、mruby-mix-miquire-fsが必要。
 * @param loadpath 追加するエントリ
 */
MRB_API void mix_miquire_append_loadpath_cstr(mrb_state *mrb, const char *loadpath);

#endif
