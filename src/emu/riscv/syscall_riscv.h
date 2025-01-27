#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdbool.h>
#include <stdint.h>

#include "riscv.h"

// Syscall number is passed in a7.
// Syscall arguments are passed in a0 to a5.
// Unused arguments are set to 0. (Legacy, and can be ignored for eventual performance gain).
// Return value is returned in a0.
void handle_syscall(riscv_t* emu, const uint64_t num);

#endif
