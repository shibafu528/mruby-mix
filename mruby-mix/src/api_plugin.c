#include <string.h>
#include <mruby/mix.h>
#include <mruby/array.h>
#include <mruby/proc.h>

MRB_API mrb_value mix_plugin_get(mrb_state *mrb, const char *slug) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_slug = mrb_symbol_value(mrb_intern_cstr(mrb, slug));
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "[]"), 1, &sym_slug);
}

MRB_API mrb_value mix_plugin_call_arg0(mrb_state *mrb, const char *event_name) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "call"), 1, &sym_event_name);
}

MRB_API mrb_value mix_plugin_call_arg1(mrb_state *mrb, const char *event_name, mrb_value arg) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value args[] = { sym_event_name, arg };
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "call"), 2, args);
}

MRB_API mrb_value mix_plugin_call_argv(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value args[argc + 1];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  memcpy(&args[1], argv, sizeof(mrb_value) * argc);
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "call"), argc + 1, args);
}

MRB_API mrb_value mix_plugin_filtering_arg1(mrb_state *mrb, const char *event_name, mrb_value arg) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value args[] = { sym_event_name, arg };
  mrb_value results = mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "filtering"), 2, args);
  if (mrb_array_p(results)) {
    return mrb_ary_entry(results, 0);
  }
  return results;
}

MRB_API mrb_value mix_plugin_filtering_argv(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value args[argc + 1];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  memcpy(&args[1], argv, sizeof(mrb_value) * argc);
  return mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "filtering"), argc + 1, args);
}

MRB_API mrb_value mix_plugin_filtering_and_nth(mrb_state *mrb, const char *event_name, mrb_int argc, const mrb_value *argv, mrb_int nth_result) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value args[argc + 1];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  memcpy(&args[1], argv, sizeof(mrb_value) * argc);
  mrb_value results = mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "filtering"), argc + 1, args);
  if (mrb_array_p(results)) {
    return mrb_ary_entry(results, nth_result);
  }
  return results;
}

MRB_API mrb_value mix_plugin_event_env(mrb_state *mrb, mix_event_env_t item) {
  if (item < 0 || MIX_EVENT_ENV_LENGTH <= item) {
    mrb_raise(mrb, E_RANGE_ERROR, "item out of range");
  }

  return mrb_proc_cfunc_env_get(mrb, item);
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

MRB_API mrb_value mix_plugin_add_event_listener_proc(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_value proc) {
  int ai = mrb_gc_arena_save(mrb);

  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value listener = mrb_funcall_with_block(mrb, plugin, mrb_intern_lit(mrb, "add_event"), 1, &sym_event_name, proc);

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

MRB_API mrb_value mix_plugin_add_event_filter_proc(mrb_state *mrb, mrb_value plugin, const char *event_name, mrb_value proc) {
  int ai = mrb_gc_arena_save(mrb);

  mrb_value sym_event_name = mrb_symbol_value(mrb_intern_cstr(mrb, event_name));
  mrb_value filter = mrb_funcall_with_block(mrb, plugin, mrb_intern_lit(mrb, "add_event_filter"), 1, &sym_event_name, proc);

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, filter);
  return filter;
}
