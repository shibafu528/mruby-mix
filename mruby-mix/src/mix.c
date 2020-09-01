#include <stdint.h>
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

static mrb_int fnv_1(mrb_state *mrb, const char *input, mrb_int input_length, mrb_int digest_length) {
  uint64_t hash, prime;
  switch (digest_length) {
  case 32:
    hash = 2166136261U;
    prime = 16777619U;
    break;
  case 64:
    hash = 14695981039346656037U;
    prime = 1099511628211LLU;
    break;
  default:
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid length for FNV");
    break;
  }

  for (uint64_t i = 0; i < input_length; i++) {
    hash = (hash * prime) ^ input[i];
  }

  return (mrb_int) hash;
}

static mrb_value unsecure_random_fnv(mrb_state *mrb, mrb_value self) {
  char *input;
  mrb_int input_length;
  mrb_int digest_length;
  if (mrb_get_args(mrb, "s|i", &input, &input_length, &digest_length) == 1) {
    digest_length = 32;
  }

  return mrb_fixnum_value(fnv_1(mrb, input, input_length, digest_length));
}

void mrb_mruby_mix_gem_init(mrb_state *mrb) {
  struct RClass *mod_mix = mrb_define_module(mrb, "Mix");
  mrb_define_module_function(mrb, mod_mix, "log", mix_log, MRB_ARGS_REQ(1));

  struct RClass *mod_unsecure_random = mrb_define_module(mrb, "UnsecureRandom");
  mrb_define_module_function(mrb, mod_unsecure_random, "_fnv", unsecure_random_fnv, MRB_ARGS_ARG(1, 1));

  MixGlobalState *mix_global_state = (MixGlobalState*) mrb_malloc(mrb, sizeof(MixGlobalState));
  memset(mix_global_state, 0, sizeof(MixGlobalState));
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "MixGlobalState"), mrb_cptr_value(mrb, mix_global_state));
}

void mrb_mruby_mix_gem_final(mrb_state *mrb) {
  MixGlobalState *mix_global_state = MIX_GS(mrb);
  mrb_free(mrb, mix_global_state);
  mrb_gv_remove(mrb, mrb_intern_lit(mrb, "MixGlobalState"));
}
