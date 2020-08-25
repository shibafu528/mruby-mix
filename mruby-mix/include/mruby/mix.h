#ifndef MRUBY_MIX_H
#define MRUBY_MIX_H

#include <mruby.h>

typedef void (*mix_log_handler)(const char *msg);

void MRB_API mix_init(mrb_state *mrb);
void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler);
void MRB_API mix_load_plugin(mrb_state *mrb, char *path);

#endif
