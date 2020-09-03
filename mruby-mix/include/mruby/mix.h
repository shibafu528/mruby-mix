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

#endif
