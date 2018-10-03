/**
 * @file stack_guard.c
 * @author Seth McBee
 * @date 2018-4-13
 * @brief GCC stack guard support - implementation.
 */
 
#include <globals.h>

#include <kernel.h>
#include <stack_guard.h>

//#ifdef STACK_GUARD

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    kernel_panic("Stack smashing detected.");
}

//#endif // STACK_GUARD
