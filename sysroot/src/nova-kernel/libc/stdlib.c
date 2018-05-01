/*
 * stdlib.c
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

#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <stdlib.h>

#include <globals.h>

int abs(int n)
{
    if (n < 0)
    {
        return (0 - n);
    }
    else
    {
        return (n);
    }
}

#if 0
double atof(const char* s)
{
    volatile double val = 0;
    size_t i = 0;

    /* Ignores any whitespace at beginning of string. */
    while (isspace(s[i]) != 0)
    {
        ++i;
    }

    /* If sign is 1, integer is positive. If sign is -1, integer is
     * negative. Default is positive. */
    double val_sign = 1;

    /* Checks for negative sign. */
    if (s[i] == '-')
    {
        val_sign = -1;
        ++i;
    }
    /* Checks for positive sign. */
    else
    {
        if (s[i] == '+')
        {
        ++i;
        }
    }

    /* Converts ASCII digits to double. */
    while (isdigit(s[i]) != 0)
    {
        val *= 10;
        val += s[i] - '0';
        ++i;
    }

    /* Checks for decimal point. */
    if (s[i] == '.')
    {
        ++i;
    }

    double decimal = 10; /* Value to multiply fractional part by. */
    size_t decimal_place = 1;

    /* Converts ASCII digits to fractional part of double. */
    while ((isdigit(s[i]) != 0) && (decimal_place <= DBL_DIG))
    {
        val += (s[i] - '0') / decimal;
        decimal *= 10;
        ++decimal_place;
        ++i;
    }

    return (val_sign * val);
}
#endif

int atoi(const char* s)
{
    int val = 0;
    size_t i = 0;

    /* If sign is 1, integer is positive. If sign is -1, integer is
     * negative. Default is positive. */
    int sign = 1;

    /* Ignores any whitespace at beginning of string. */
    while (isspace(s[i]) != 0)
    {
        ++i;
    }

    /* Checks for negative sign. */
    if (s[i] == '-')
    {
        sign = -1;
        ++i;
    }
    /* Checks for positive sign. */
    else
    {
        if (s[i] == '+')
        {
            ++i;
        }
    }

    /* Converts ASCII digits to int. */
    while (isdigit(s[i]) != 0)
    {
        val = val * 10;
        val += s[i] - '0';
        ++i;
    }

    return (sign * val);
}

long atol(const char* s)
{
    long val = 0;
    size_t i = 0;

    /* If sign is 1, integer is positive. If sign is -1, integer is
     * negative. Default is positive. */
    long sign = 1;

    /* Ignores any whitespace at beginning of string. */
    while (isspace(s[i]) != 0)
    {
        ++i;
    }

    /* Checks for negative sign. */
    if (s[i] == '-')
    {
        sign = -1;
        ++i;
    }
    /* Checks for positive sign. */
    else
    {
        if (s[i] == '+')
        {
            ++i;
        }
    }

    /* Converts ASCII digits to long. */
    while (isdigit(s[i]) != 0)
    {
        val *= 10;
        val += s[i] - '0';
        ++i;
    }

    return (sign * val);
}

char* btoa(uint8_t val, char* str, size_t bits)
{
    const char digit[] = "01";
	size_t str_i = 0;
    size_t i = 0;

    if (bits > 8)
    {
        bits = 8;
    }

	while (str_i < bits)
	{
        str[str_i] = '0';
        ++str_i;
        ++i;
	}

	str[str_i] = '\0';

	while (val > 0)
	{
        --str_i;

		str[str_i] = digit[val % 2];
		val /= 2;
	}

	return (str);
}

char* itoa(int val, char* str, int base)
{
	const char digit[] = "0123456789ABCDEF";
	size_t str_i = 0;

	if (val < 0)
	{
		str[str_i] = '-';
		++str_i;
		val *= -1;
	}

	switch (base)
	{
	case (2):
		str[str_i] = '0';
		++str_i;
		str[str_i] = 'b';
		++str_i;
		break;

	case (8):
		str[str_i] = '0';
		++str_i;
		break;

	case (10):
		break;

	case (16):
		str[str_i] = '0';
		++str_i;
		str[str_i] = 'x';
		++str_i;
		break;

	default:
		str = "ERROR: BASE NOT SUPPORTED";
		return (str);
	}

	if (val == 0)
	{
		str[str_i] = digit[0];
        str[str_i + 1] = '\0';
		return (str);
	}

	size_t place = 0;
	int place_val = val;

	while (place_val > 0)
	{
		++place;
		place_val /= base;
	}

	str_i += place;

	str[str_i] = '\0';

	while (val > 0)
	{
		--str_i;
		str[str_i] = digit[val%base];
		val /= base;
	}

	return (str);
}

long labs(long n)
{
    if (n < 0)
    {
        return (0 - n);
    }
    else
    {
        return (n);
    }
}

char* litoa(long val, char* str, int base)
{
	const char digit[] = "0123456789ABCDEF";
	size_t str_i = 0;

	if (val < 0)
	{
		str[str_i] = '-';
		++str_i;
		val *= -1;
	}

	switch (base)
	{
	case (2):
		str[0] = '0';
		str[1] = 'b';
		str_i += 2;
		break;

	case (8):
		str[0] = '0';
		++str_i;
		break;

	case (10):
		break;

	case (16):
		str[0] = '0';
		str[1] = 'x';
		str_i += 2;
		break;

	default:
		str = "ERROR: BASE NOT SUPPORTED";
		return (str);
	}

	if (val == 0)
	{
		str[str_i] = digit[0];
        str[str_i + 1] = '\0';
		return (str);
	}

	size_t place = 0;
	long place_val = val;

	while (place_val > 0)
	{
		++place;
		place_val /= base;
	}

	str_i += place;

	str[str_i] = '\0';

	while (val > 0)
	{
		--str_i;
		str[str_i] = digit[val%base];
		val /= base;
	}

	return (str);
}

char* sitoa(size_t val, char* str, int base)
{
	const char digit[] = "0123456789ABCDEF";
	size_t str_i = 0;

	switch (base)
	{
	case (2):
		str[0] = '0';
		str[1] = 'b';
		str_i += 2;
		break;

	case (8):
		str[0] = '0';
		++str_i;
		break;

	case (10):
		break;

	case (16):
		str[0] = '0';
		str[1] = 'x';
		str_i += 2;
		break;

	default:
		str = "ERROR: BASE NOT SUPPORTED";
		return (str);
	}

	if (val == 0)
	{
		str[str_i] = digit[0];
        str[str_i + 1] = '\0';
		return (str);
	}

	size_t place = 0;
	size_t place_val = val;

	while (place_val > 0)
	{
		++place;
		place_val /= base;
	}

	str_i += place;

	str[str_i] = '\0';

	while (val > 0)
	{
		--str_i;
		str[str_i] = digit[val%base];
		val /= base;
	}

	return (str);
}

static unsigned long int rand_seed = 1;
int rand_max(unsigned int max) // RAND_MAX assumed to be 32767
{
    rand_seed = rand_seed * 1103515245 + 12345;
    return (unsigned int)(rand_seed / 65536) % (max + 1);
}
void srand( unsigned int seed )
{
    rand_seed = seed;
}
