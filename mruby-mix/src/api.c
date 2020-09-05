#include <stdio.h>
#include <mruby/mix.h>
#include <mruby/hash.h>
#include <mruby/class.h>
#include <mruby/proc.h>
#include "mix_internal.h"

#define mrb_intern_cstr_v(mrb, str) mrb_symbol_value(mrb_intern_cstr(mrb, str))
#define mrb_intern_lit_v(mrb, lit) mrb_symbol_value(mrb_intern_lit(mrb, lit))

static mrb_value remain_handler(mrb_state *mrb, mrb_value self) {
  MixGlobalState *gs = MIX_GS(mrb);
  if (gs->remain_handler) {
    gs->remain_handler(mrb);
  }
  return mrb_nil_value();
}

static mrb_value reserve_handler(mrb_state *mrb, mrb_value self) {
  MixGlobalState *gs = MIX_GS(mrb);
  if (gs->reserve_handler) {
    mrb_float delay = 0;
    mrb_get_args(mrb, "f", &delay);
    gs->reserve_handler(mrb, delay);
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

void MRB_API mix_register_reserve_handler(mrb_state *mrb, mix_reserve_handler handler) {
  MixGlobalState *gs = MIX_GS(mrb);
  gs->reserve_handler = handler;

  struct RClass *mod_delayer = mrb_module_get(mrb, "Delayer");
  struct RProc *proc = mrb_proc_new_cfunc(mrb, reserve_handler);
  mrb_funcall_with_block(mrb,
                         mrb_obj_value(mod_delayer),
                         mrb_intern_lit(mrb, "register_reserve_hook"),
                         0, NULL,
                         mrb_obj_value(proc));
}

void MRB_API mix_register_log_handler(mrb_state *mrb, mix_log_handler handler) {
  MixGlobalState *gs = MIX_GS(mrb);
  gs->log_handler = handler;
}

int MRB_API mix_require(mrb_state *mrb, const char *path) {
  int ai = mrb_gc_arena_save(mrb);
  mrb_value str_path = mrb_str_new_cstr(mrb, path);
  mrb_value result = mrb_funcall_argv(mrb, mrb_obj_value(mrb->kernel_module), mrb_intern_lit(mrb, "require"), 1, &str_path);
  mrb_gc_arena_restore(mrb, ai);
  return mrb->exc == NULL && mrb_true_p(result);
}

MRB_API struct RClass* mix_class_dig(mrb_state *mrb, size_t count, const char **names) {
  mrb_value klass = mrb_obj_value(mrb->object_class);
  for (size_t i = 0; i < count; i++) {
    klass = mrb_const_get(mrb, klass, mrb_intern_cstr(mrb, names[i]));
  }

  mrb_check_type(mrb, klass, MRB_TT_CLASS);
  return mrb_class_ptr(klass);
}

MRB_API struct RClass* mix_module_dig(mrb_state *mrb, size_t count, const char **names) {
  mrb_value klass = mrb_obj_value(mrb->object_class);
  for (size_t i = 0; i < count; i++) {
    klass = mrb_const_get(mrb, klass, mrb_intern_cstr(mrb, names[i]));
  }

  mrb_check_type(mrb, klass, MRB_TT_MODULE);
  return mrb_class_ptr(klass);
}

mrb_value MRB_API mix_gui_event_new(mrb_state *mrb, const char *event, mrb_value widget, mrb_value messages, mrb_value world) {
  struct RClass *cls_event = mix_class_get(mrb, "Plugin", "GUI", "Event");
  mrb_value args = mrb_hash_new(mrb);
  mrb_hash_set(mrb, args, mrb_intern_lit_v(mrb, "event"), mrb_intern_cstr_v(mrb, event));
  mrb_hash_set(mrb, args, mrb_intern_lit_v(mrb, "widget"), widget);
  mrb_hash_set(mrb, args, mrb_intern_lit_v(mrb, "messages"), messages);
  mrb_hash_set(mrb, args, mrb_intern_lit_v(mrb, "world"), world);
  return mrb_obj_new(mrb, cls_event, 1, &args);
}
