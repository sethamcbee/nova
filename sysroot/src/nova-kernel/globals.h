/*
 * globals.h
 *
 * Copyright 2014 Seth Nils <altindiefanboy@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

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
#define BIT_SET(x,bit) (x | (1 << bit))

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
#define BIT_CLEAR(x,bit) (x & ~(1 << bit))

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
#define BIT_TOGGLE(x,bit) (x ^ (1 << bit))

/**
 * @brief Checks a bit, returning it's value.
 *
 * @param x Variable containing bit to check.
 * @param bit Bit to check.
 *
 * @return Value of given bit.
 */
#define BIT_CHECK(x,bit) ((x & (1 << bit)) ? 1 : 0)

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

#endif /* GLOBALS_H_INCLUDED */
