#include <mruby/mix.h>
#include <mruby/proc.h>
#include "mix_internal.h"

MRB_API mrb_value mix_plugin_get(mrb_state *mrb, const char *slug) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_slug = mrb_symbol_value(mrb_intern_cstr(mrb, slug));
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "[]"), 1, &sym_slug);
}

MRB_API mrb_value mix_plugin_event_get_receiver_from_env(mrb_state *mrb) {
  return mrb_proc_cfunc_env_get(mrb, 0);
}

MRB_API mrb_value mix_plugin_event_get_name_from_env(mrb_state *mrb) {
  return mrb_proc_cfunc_env_get(mrb, 1);
}

MRB_API mrb_value mix_plugin_add_event_listener(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_func_t callback) {
  int ai = mrb_gc_arena_save(mrb);

  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value env[] = { plugin, sym_event_name };
  struct RProc *proc = mrb_proc_new_cfunc_with_env(mrb, callback, sizeof(env) / sizeof(mrb_value), env);
  mrb_value listener = mrb_funcall_with_block(mrb, plugin, mrb_intern_lit(mrb, "add_event"), 1, &sym_event_name, mrb_obj_value(proc));

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, listener);
  return listener;
}

MRB_API mrb_value mix_plugin_add_event_filter(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_func_t callback) {
  int ai = mrb_gc_arena_save(mrb);

  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value env[] = { plugin, sym_event_name };
  struct RProc *proc = mrb_proc_new_cfunc_with_env(mrb, callback, sizeof(env) / sizeof(mrb_value), env);
  mrb_value filter = mrb_funcall_with_block(mrb, plugin, mrb_intern_lit(mrb, "add_event_filter"), 1, &sym_event_name, mrb_obj_value(proc));

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, filter);
  return filter;
}
