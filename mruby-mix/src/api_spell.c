#include <string.h>
#include <mruby/mix.h>
#include <mruby/hash.h>

static void assert_plugin_loaded(mrb_state *mrb, const char *plugin_slug) {
  struct RClass *cls_plugin = mrb_class_get(mrb, "Plugin");
  mrb_value sym_slug = mrb_symbol_value(mrb_intern_cstr(mrb, plugin_slug));
  mrb_value result = mrb_funcall_argv(mrb, mrb_obj_value(cls_plugin), mrb_intern_lit(mrb, "instance_exist?"), 1, &sym_slug);
  if (!mrb_true_p(result)) {
    mrb_raisef(mrb, E_KEY_ERROR, "Plugin `%s' is not loaded", plugin_slug);
  }
}

MRB_API mrb_value mix_spell(mrb_state *mrb, const char *spell_name, mrb_int argc, const mrb_value *argv) {
  assert_plugin_loaded(mrb, "spell");

  mrb_value plugin = mix_plugin_get(mrb, "spell");
  mrb_value args[argc + 1];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, spell_name));
  memcpy(&args[1], argv, sizeof(mrb_value) * argc);
  return mrb_funcall_argv(mrb, plugin, mrb_intern_lit(mrb, "spell"), argc + 1, args);
}

MRB_API mrb_bool mix_spell_defined(mrb_state *mrb, const char *spell_name, mrb_int argc, const mrb_value *argv) {
  assert_plugin_loaded(mrb, "spell");

  mrb_value plugin = mix_plugin_get(mrb, "spell");
  mrb_value args[argc + 1];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, spell_name));
  memcpy(&args[1], argv, sizeof(mrb_value) * argc);
  mrb_value result = mrb_funcall_argv(mrb, plugin, mrb_intern_lit(mrb, "spell?"), argc + 1, args);
  return mrb_test(result);
}

MRB_API mrb_value mix_define_spell(mrb_state *mrb, const char *spell_name, mrb_int slug_count, const mrb_sym *slugs, mrb_value condition, mrb_value body) {
  assert_plugin_loaded(mrb, "spell");

  mrb_value plugin = mix_plugin_get(mrb, "spell");
  mrb_value args[slug_count + 2];
  args[0] = mrb_symbol_value(mrb_intern_cstr(mrb, spell_name));
  for (int i = 0; i < slug_count; i++) {
    args[i + 1] = mrb_symbol_value(slugs[i]);
  }
  args[slug_count + 1] = mrb_hash_new(mrb);
  if (!mrb_nil_p(condition)) {
    mrb_hash_set(mrb, args[slug_count + 1], mrb_symbol_value(mrb_intern_lit(mrb, "condition")), condition);
  }

  mrb_value result = mrb_funcall_with_block(mrb, plugin, mrb_intern_lit(mrb, "defspell"), slug_count + 2, args, body);

  if (!mrb_exception_p(result)) {
    return mrb_true_value();
  }
  return result;
}
