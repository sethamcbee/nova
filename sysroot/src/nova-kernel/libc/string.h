/*
 * string.h
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

#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Copies string ct to string s, including terminating character '\0'.
 *
 * @param s Destination string.
 * @param ct Source string.
 *
 * @return String s replaced with string ct.
 */
char* strcpy(char* s, const char* ct);

/**
 * @brief Copies, at most, n characters of string ct to string s.
 *
 * @param s Destination string.
 * @param ct Source string.
 * @param n Maximum number of characters to copy.
 *
 * @return String s replaced string ct up until maximum
 * number of characters n.
 *
 * @note If ct is shorter than n, the s will be padded with '\0's
 * until it reaches length n.
 */
char* strncpy(char* s, const char* ct, size_t n);

/**
 * @brief Concatenates string ct to end of string s.
 *
 * @param s Destination string.
 * @param ct Source string.
 *
 * @return String s with string ct concatenated to the end.
 *
 * @warning The two strings may not overlap.
 */
char* strcat(char* s, const char* ct);

/**
 * @brief Concatenates, at most, n characters of string ct to end of string
 * s.
 *
 * @param s Destination string.
 * @param ct Source string.
 * @param n Maximum number of characters to append.
 *
 * @return String s with string ct appended to the end, up until maximum
 * number of characters n, terminated with '\0'.
 *
 * @warning The two strings may not overlap.
 */
char* strncat(char* s, const char* ct, size_t n);

/**
 * @brief Compares string cs to string ct.
 *
 * @param cs String to be compared.
 * @param ct String to be compared.
 *
 * @return Binary comparison of string cs to string ct. Returns 0 if
 * cs == ct, > 0 if the first character in which cs is different than ct is
 * greater than the equivalent character of string ct, or > 0 if the
 * first character in which cs is different than ct is less than the
 * equivalent character of string ct.
 */
int strcmp(const char* cs, const char* ct);

/**
 * @brief Compares at most n characters of string cs to string ct.
 *
 * @param cs String to be compared.
 * @param ct String to be compared.
 *
 * @return Binary comparison of string cs to string ct, up until character
 * number n. Returns 0 if cs == ct, > 0 if the first
 * character in which cs is different than ct is greater than the
 * equivalent character of string ct, or < 0 if the first
 * character in which cs is different than ct is less than the equivalent
 * character of string ct.
 */
int strncmp(const char* cs, const char* ct, size_t n);

/**
 * @brief Finds first occurrence of character c in string cs, if any.
 *
 * @param cs String to search in.
 * @param c Character to search for.
 *
 * @return Pointer to first occurrence of character c in string cs. If c
 * is not present in cs, return NULL.
 *
 * @note c is an unsigned character promoted to int, internally treated
 * as char.
 */
const char* strchr(const char* cs, int c);

/**
 * @brief Finds last occurrence of character c in string cs, if any.
 *
 * @param cs String to search in.
 * @param c Character to search for.
 *
 * @return Pointer to last occurrence of character c in string cs. If c
 * is not present in cs, return NULL.
 *
 * @note c is an unsigned character promoted to int, internally treated
 * as char.
 */
const char* strrchr(const char* cs, int c);

/**
 * @brief Returns length of string cs starting from the beginning consisting
 * of only characters that appear in string ct.
 *
 * @param cs String to search in.
 * @param ct String containing characters to search for.
 *
 * @return Length of the beginning of string cs containing only of
 * characters that appear in string ct. If the first character of cs is
 * not found in ct, the function returns 0.
 */
size_t strspn(const char* cs, const char* ct);

/**
 * @brief Returns length of string cs starting from the beginning consisting
 * of characters not found in string ct.
 *
 * @param ct String to search in.
 * @param cs String containing characters to search for.
 *
 * @return Length of the beginning of string cs containing only of
 * characters that do NOT appear in string ct. If the first character of cs
 * is found in ct, the function returns 0.
 */
size_t strcspn(const char* cs, const char* ct);

/**
 * @brief Finds first occurrence in string cs of any character found in string
 * ct, if any.
 *
 * @param cs String to search in.
 * @param ct String to search for.
 *
 * @return Pointer to first occurence in string cs of any character found in
 * string ct, returning NULL if no characters in string ct are present in
 * string cs.
 */
const char* strpbrk(const char* cs, const char* ct);

/**
 * @brief Finds first occurrence of string ct in string cs, if any.
 *
 * @param cs String to search in.
 * @param ct String to search for.
 *
 * @return Pointer to first occurrence of string ct found in string cs,
 * returning NULL if ct is not present in cs at all.
 */
const char* strstr(const char* cs, const char* ct);

/**
 * @brief Returns length of string cs.
 *
 * @param cs String to find length of.
 *
 * @return Length of string cs, terminated at '\0'. Length does not include
 * terminating character.
 */
size_t strlen(const char* cs);

/**
 * @brief Copy a specified number of objects treated as characters from
 * one location to another.
 *
 * @param s Destination.
 * @param ct Source.
 * @param n Number of characters to copy.
 *
 * @return Pointer to s.
 */
void* memcpy(void* s, const void* ct, size_t n);

/**
 * @brief Copies a specified number of characters from one location to another
 * and guaranteed to work properly even if the objects overlap.
 *
 * @param s Destination.
 * @param ct Source.
 * @param n Number of characters to copy.
 *
 * @return Pointer to s.
 */
void* memmove(void* s, const void* ct, size_t n);

/**
 * @brief Compares at most n characters of pointer cs to pointer ct.
 *
 * @param cs Pointer to be compared.
 * @param ct Pointer to be compared.
 *
 * @return Binary comparison of pointer cs to pointer ct, up until character
 * number n. Returns 0 if cs == ct, > 0 if the first
 * character in which cs is different than ct is greater than the
 * equivalent character of string ct, or < 0 if the first
 * character in which cs is different than ct is less than the equivalent
 * character of string ct.
 */
int memcmp(const void* cs, const void* ct, size_t n);

/**
 * @brief Return pointer to first occurence of character c in cs.
 *
 * @param cs Pointer to beginning of memory to search.
 * @param c Character to search for.
 * @param n Number of memory locations to search.
 *
 * @return Return pointer to first occurence of character c in cs, or NULL
 * if not present.
 */
void* memchr(const void* cs, int c, size_t n);

/**
 * @brief Places character c into first n characters starting at location
 * s.
 *
 * @param s Pointer to beginning of memory to place characters in.
 * @param c Character to place.
 * @param n Number of characters to place.
 */
void* memset(void* s, int c, size_t n);

/* The following are not found in the standard library, bu have still been
 * implemented because they are commonly supported and used and/or fit the
 * purpose of this header.
 */

/**
 * @brief Reverses a string.
 *
 * @param s String to place reversed string in.
 * @param ct String to reverse.
 *
 * @note This will work even if the strings overlap or are the same string.
 */
char* strrev(char* s, const char* ct);

char * strtok (char *s, const char *delim);

#endif /* STRING_H */
