/**
 * @file assert.h
 * @author Seth McBee
 * @date 2018-9-5
 * @brief Compile-time and run-time assertions.
 */

#ifndef ASSERT_H
#define ASSERT_H

#include <globals.h>

#include <stdlib.h>
#include <string.h>

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Compiler built-in static assertion.
 */
#define static_assert _Static_assert

/**
 * @brief Handle kernel run-time assertion.
 *
 * @param file Name of the file in which the assertion failed.
 * @param func Name of the function in which the assertion failed.
 * @param line Line number at which the assertion failed.
 * @param exp Expression that failed evaluation.
 */
void kassert(const char* file, const char* func, long line, const char* exp);

#ifndef NDEBUG
/**
 * @brief Run-time assertion.
 */
#define assert(x) ((x) ? 0 : kassert(__FILE__, __func__, __LINE__, #x))
#else
#define assert(ignore) (0)
#endif // DEBUG

#ifdef __cplusplus
}
#endif

#endif // ASSERT_H
