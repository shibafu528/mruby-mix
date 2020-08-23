#include <stdlib.h>
#include <stdio.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/mix.h>

int main(int argc, char* argv[]) {
  mrb_state *mrb = mrb_open();
  int exitcode = EXIT_SUCCESS;

  mix_init(mrb);
  //mrb_load_string(mrb, "p HYDE");

  if (mrb->exc) {
    mrb_print_error(mrb);
    exitcode = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return exitcode;
}
