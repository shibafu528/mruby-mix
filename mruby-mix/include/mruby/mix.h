#ifndef MRUBY_MIX_H
#define MRUBY_MIX_H

#include <mruby.h>

void MRB_API mix_init(mrb_state *mrb);
void MRB_API mix_load_plugin(mrb_state *mrb, char *path);

#endif
