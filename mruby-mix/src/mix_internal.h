#ifndef MIX_INTERNAL_H
#define MIX_INTERNAL_H

#include <mruby/mix.h>
#include <mruby/variable.h>

typedef struct {
  mix_remain_handler remain_handler;
  mix_log_handler log_handler;
} MixGlobalState;

#define MIX_GS(mrb) ((MixGlobalState*) mrb_cptr(mrb_gv_get(mrb, mrb_intern_lit(mrb, "MixGlobalState"))))

#endif
