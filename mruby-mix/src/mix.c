#include <mruby.h>
#include <mruby/string.h>
#include <memory.h>
#include "mix_internal.h"

static mrb_value mix_log(mrb_state *mrb, mrb_value self) {
  MixGlobalState *gs = MIX_GS(mrb);
  if (!gs->log_handler) {
    return mrb_nil_value();
  }

  mrb_value msg;
  mrb_get_args(mrb, "o", &msg);

  char *c_msg = mrb_str_to_cstr(mrb, msg);
  gs->log_handler(c_msg);

  return mrb_nil_value();
}

void mrb_mruby_mix_gem_init(mrb_state *mrb) {
  struct RClass *mod_mix = mrb_define_module(mrb, "Mix");

  mrb_define_module_function(mrb, mod_mix, "log", mix_log, MRB_ARGS_REQ(1));

  MixGlobalState *mix_global_state = (MixGlobalState*) mrb_malloc(mrb, sizeof(MixGlobalState));
  memset(mix_global_state, 0, sizeof(MixGlobalState));
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "MixGlobalState"), mrb_cptr_value(mrb, mix_global_state));
}

void mrb_mruby_mix_gem_final(mrb_state *mrb) {
  MixGlobalState *mix_global_state = MIX_GS(mrb);
  mrb_free(mrb, mix_global_state);
  mrb_gv_remove(mrb, mrb_intern_lit(mrb, "MixGlobalState"));
}
