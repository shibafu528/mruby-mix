#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/mix.h>

static int remain = 0;

void remain_handler() {
  printf("[MIX_REMAIN] Callback remain_handler.\n");
  remain++;
}

void log_handler(const char* msg) {
  printf("[MIX_LOG] %s\n", msg);
}

int main(int argc, char* argv[]) {
  mrb_state *mrb = mrb_open();
  int exitcode = EXIT_SUCCESS;

  mix_register_remain_handler(mrb, remain_handler);
  mix_register_log_handler(mrb, log_handler);

  mrb_load_string(mrb,
                  "Plugin.create :sample do\n"
                  "  on_too_late do\n"
                  "    warn 'Never too late!!'\n"
                  "  end\n"
                  "end"
                  );
  mrb_load_string(mrb, "Plugin.call(:too_late)");

  while (1) {
    if (remain--) {
      mix_run(mrb);
    }

    if (mrb->exc) {
      mrb_print_error(mrb);
      exitcode = EXIT_FAILURE;
      break;
    }

    sleep(1);
  }

  mrb_close(mrb);
  return exitcode;
}
