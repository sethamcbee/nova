/**
 * @file stack_guard.cc
 * @author Seth McBee
 * @date 2018-4-13
 * @brief GCC stack guard support - implementation.
 */

#ifdef STACK_GUARD

#include <kernel.h>
#include <stack_guard.h>

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

extern "C" void __stack_chk_fail(void)
{
    kernel_panic("Stack smashing detected.");
}

#endif // STACK_GUARD
