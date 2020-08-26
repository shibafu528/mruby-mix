#include <mruby/mix.h>
#include <mruby/proc.h>
#include "mix_internal.h"

static mrb_value remain_handler(mrb_state *mrb, mrb_value self) {
  MixGlobalState *gs = MIX_GS(mrb);
  if (gs->remain_handler) {
    gs->remain_handler();
  }
  return mrb_nil_value();
}

void MRB_API mix_run(mrb_state *mrb) {
  struct RClass *mod_delayer = mrb_module_get(mrb, "Delayer");
  mrb_value mod_delayer_val = mrb_obj_value(mod_delayer);
  mrb_sym sym_run = mrb_intern_lit(mrb, "run");
  mrb_sym sym_empty_p = mrb_intern_lit(mrb, "empty?");
  int ai = mrb_gc_arena_save(mrb);

  while (1) {
    mrb_funcall_argv(mrb, mod_delayer_val, sym_run, 0, NULL);

    mrb_value empty_p = mrb_funcall_argv(mrb, mod_delayer_val, sym_empty_p, 0, NULL);
    if (mrb_bool(empty_p)) {
      break;
    }

    mrb_gc_arena_restore(mrb, ai);
  }

  mrb_gc_arena_restore(mrb, ai);
}

void MRB_API mix_register_remain_handler(mrb_state *mrb, mix_remain_handler handler) {
  MixGlobalState *gs = MIX_GS(mrb);
  gs->remain_handler = handler;

  struct RClass *mod_delayer = mrb_module_get(mrb, "Delayer");
  struct RProc *proc = mrb_proc_new_cfunc(mrb, remain_handler);
  mrb_funcall_with_block(mrb,
                         mrb_obj_value(mod_delayer),
                         mrb_intern_lit(mrb, "register_remain_hook"),
                         0, NULL,
                         mrb_obj_value(proc));
}

void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler) {
  MixGlobalState *gs = MIX_GS(mrb);
  gs->log_handler = handler;
}

void MRB_API mix_load_plugin(mrb_state *mrb, char *path) {
}
