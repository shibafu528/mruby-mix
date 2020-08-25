#include <stdlib.h>
#include <stdio.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/mix.h>

void log_handler(const char* msg) {
  printf("[MIX_LOG] %s\n", msg);
}

int main(int argc, char* argv[]) {
  mrb_state *mrb = mrb_open();
  int exitcode = EXIT_SUCCESS;

  mix_register_log_handler(mrb, log_handler);

  mrb_load_string(mrb, "warn 'Never too late!!'");
  //mrb_load_string(mrb, "p HYDE");

  if (mrb->exc) {
    mrb_print_error(mrb);
    exitcode = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return exitcode;
}
