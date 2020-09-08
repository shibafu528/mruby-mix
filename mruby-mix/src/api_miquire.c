#include <mruby/mix.h>
#include <mruby/array.h>
#include <mruby/hash.h>

MRB_API mrb_value mix_miquire_load_all(mrb_state *mrb) {
  struct RClass *miquire = mix_module_get(mrb, "Mix", "Miquire");
  return mrb_funcall_argv(mrb, mrb_obj_value(miquire), mrb_intern_lit(mrb, "load_all"), 0, NULL);
}

MRB_API mrb_value mix_miquire_load_all_failfast(mrb_state *mrb) {
  struct RClass *miquire = mix_module_get(mrb, "Mix", "Miquire");
  mrb_value kwargs = mrb_hash_new(mrb);
  mrb_hash_set(mrb, kwargs, mrb_symbol_value(mrb_intern_lit(mrb, "failfast")), mrb_true_value());
  return mrb_funcall_argv(mrb, mrb_obj_value(miquire), mrb_intern_lit(mrb, "load_all"), 1, &kwargs);
}

MRB_API void mix_miquire_append_loadpath(mrb_state *mrb, mrb_value loadpath) {
  struct RClass *miquire = mix_module_get(mrb, "Mix", "Miquire");
  mrb_value loadpaths = mrb_funcall_argv(mrb, mrb_obj_value(miquire), mrb_intern_lit(mrb, "loadpath"), 0, NULL);
  mrb_ary_push(mrb, loadpaths, loadpath);
}

MRB_API void mix_miquire_append_loadpath_cstr(mrb_state *mrb, const char *loadpath) {
  struct RClass *miquire = mix_module_get(mrb, "Mix", "Miquire");
  mrb_value loadpaths = mrb_funcall_argv(mrb, mrb_obj_value(miquire), mrb_intern_lit(mrb, "loadpath"), 0, NULL);
  mrb_ary_push(mrb, loadpaths, mrb_str_new_cstr(mrb, loadpath));
}
