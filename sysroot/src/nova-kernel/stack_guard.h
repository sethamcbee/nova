/**
 * @file stack_guard.h
 * @author Seth McBee
 * @date 2018-4-13
 * @brief GCC stack guard support.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef STACK_GUARD

#include <stdint.h>

/**
 * @brief Some arbitrary value for the stack guard implementation to
 * check for stack clobbering.
 */
#define STACK_CHK_GUARD 0x9012

/**
 * @brief Pointer used by GCC stack guard code.
 */
extern uintptr_t __stack_chk_guard;

/**
 * @brief Function called upon detection of stack corruption.
 */
void __stack_chk_fail(void);

#ifdef __cplusplus
}
#endif

#endif // STACK_GUARD
