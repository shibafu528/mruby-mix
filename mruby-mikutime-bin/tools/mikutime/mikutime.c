#include <stdlib.h>
#include <stdio.h>
#include <mruby.h>
#include <mruby/compile.h>

void MRB_API mikutime_init(mrb_state *mrb) {
  mrb_funcall(mrb, mrb_obj_value(mrb_module_get(mrb, "Mikutime")), "init!", 0);
}

void MRB_API mikutime_load_plugin(mrb_state *mrb, char *path) {
}

int main(int argc, char* argv[]) {
  mrb_state *mrb = mrb_open();
  int exitcode = EXIT_SUCCESS;

  mikutime_init(mrb);
  //mrb_load_string(mrb, "p HYDE");

  if (mrb->exc) {
    mrb_print_error(mrb);
    exitcode = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return exitcode;
}
