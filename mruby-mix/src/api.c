#include <mruby/mix.h>
#include "mix_internal.h"

void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler) {
  MixGlobalState *gs = MIX_GS(mrb);
  gs->log_handler = handler;
}

void MRB_API mix_load_plugin(mrb_state *mrb, char *path) {
}
