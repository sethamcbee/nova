// Authors: Seth McBee
// Created: 2018-4-13
// Description: GCC stack guard implementation.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <stack_guard.h>

#ifdef STACK_GUARD

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
    kernel_panic("Stack smashing detected.");
}

#endif
