#include <mruby/mix.h>

void MRB_API mix_init(mrb_state *mrb) {
  mrb_funcall(mrb, mrb_obj_value(mrb_module_get(mrb, "Mix")), "init!", 0);
}

void MRB_API mix_load_plugin(mrb_state *mrb, char *path) {
}

void mrb_mruby_mix_gem_init(mrb_state *mrb) {
}

void mrb_mruby_mix_gem_final(mrb_state *mrb) {
}
