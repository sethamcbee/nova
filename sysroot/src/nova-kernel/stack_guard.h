// Authors: Seth McBee
// Created: 2018-4-13
// Description: GCC stack guard implementation.

#ifdef STACK_GUARD

#ifndef STACK_GUARD_H
#define STACK_GUARD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Some arbitrary value.
#define STACK_CHK_GUARD 0x901249712

uintptr_t __stack_chk_guard;

__attribute__((noreturn))
void __stack_chk_fail(void);

#endif // STACK_GUARD_H

#endif // STACK_GUARD
