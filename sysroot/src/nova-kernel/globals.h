/**
 * @file globals.h
 * @author Seth McBee
 * @date 2014-?-?
 * @brief Miscellaneous typedefs and macros.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#pragma once

#ifdef __cplusplus

#include <new>

extern "C" {
#endif

typedef ptrdiff_t ssize_t;

/**
 * @brief Sets a bit, giving it a value of 1.
 *
 * @param x Variable containing bit to set.
 * @param bit Bit to set.
 *
 * @return Variable with given bit set.
 *
 * @note Does not affect original variable.
 */
#define BIT_SET(x,bit) ((x) | (1 << (bit)))

/**
 * @brief Clears a bit, giving it a value of 1.
 *
 * @param x Variable containing bit to clear.
 * @param bit Bit to clear.
 *
 * @return Variable with given bit clear.
 *
 * @note Does not affect original variable.
 */
#define BIT_CLEAR(x,bit) ((x) & ~(1 << (bit)))

/**
 * @brief Toggles a bit, inversing it's value.
 *
 * @param x Variable containing bit to toggle.
 * @param bit Bit to toggle.
 *
 * @return Variable with given bit toggle.
 *
 * @note Does not affect original variable.
 */
#define BIT_TOGGLE(x,bit) ((x) ^ (1 << (bit)))

/**
 * @brief Checks a bit, returning it's value.
 *
 * @param x Variable containing bit to check.
 * @param bit Bit to check.
 *
 * @return Value of given bit.
 */
#define BIT_CHECK(x,bit) (((x) & (1 << (bit))) ? 1 : 0)

/**
 * @brief Hints to the compiler that an expression is expected to return
 * true.
 *
 * @param x Expression to evaluate.
 */
#define LIKELY(x) __builtin_expect((x), 1)

/**
 * @brief Hints to the compiler that an expression is expected to return
 * false.
 *
 * @param x Expression to evaluate.
 */
#define UNLIKELY(x) __builtin_expect((x), 0)

#ifdef __cplusplus
}
#endif
