#include "backtrace.h"
#include "display.h"

#include <stddef.h>

extern FILE *libdheap_log;
extern void *__libc_stack_end;

/**
 * Assumes a stack layout where stack frames are linked using frame pointers
 *
 *        +-----------------+     +-----------------+
 *  FP -> | previous FP --------> | previous FP ------>
 *        |                 |     |                 |
 *        | return address  |     | return address  |
 *        +-----------------+     +-----------------+
 *
 * Stack is growing upwards, so addresses grow downwards
 * Assuming pointers to the above can be stored in size_t data type
 * Tested on 32 bit and 64 bit machines, works fine for now
 */

void set_backtrace (void **backtrace, unsigned int start, unsigned int len) {
  size_t *current_frame_address;
  size_t *current_return_address;
  unsigned int i;

  if (len == 0) {
    return;
  }

  current_frame_address = __builtin_frame_address(0);
  for (i = 0; i < (start + len); i++) {
    if (current_frame_address >= (size_t *)&i &&
      current_frame_address <= (size_t *)__libc_stack_end) {
      // current_frame_address falls in range of 'i' (lowest possible stack
      // address) and __libc_stack_end (highest possible stack address)
      current_return_address = (size_t *)*(current_frame_address + 1);
      current_frame_address = (size_t *)*(current_frame_address);
    } else {
      // End of stack frames
      current_return_address = NULL;
    }
    if (i >= start) {
      backtrace[i - start] = current_return_address;
    }
  }
}

void print_backtrace (void **backtrace, unsigned int len) {
  unsigned int i;
  display_log(libdheap_log, "Printing Stack Trace ====>");
  for (i = 0; i < len; i++) {
    if (backtrace[i] == NULL) {
      break;
    }
    display_log(libdheap_log, "\t\t%p", backtrace[i]);
  }
  display_log(libdheap_log, "<==== End of Stack Trace\n");
}
