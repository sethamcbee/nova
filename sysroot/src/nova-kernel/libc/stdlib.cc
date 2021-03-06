/**
 * @file stdlib.c
 * @author Seth McBee
 * @date 2014-?-?
 * @brief C standard library stdlib implementation.
 */


#include <globals.h>

#include <float.h>

#include <ctype.h>
#include <stdlib.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/memory/pmm.h>
#endif

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

double atof(const char* s)
{
    double val = 0;
    size_t i = 0;

    /* Ignores any whitespace at beginning of string. */
    while (isspace(s[i]) != 0)
    {
        ++i;
    }

    /* If sign is 1, integer is positive. If sign is -1, integer is
     * negative. Default is positive. */
    double val_sign = 1.0;

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
        val *= 10.0;
        val += (float)(s[i] - '0');
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

char* itoa(int val, char* str)
{
    return ( _itoa(val, str, 10) );
}

char* _itoa(int val, char* str, int base)
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

char* litoa(long val, char* str)
{
    return ( _litoa(val, str, 10) );
}

char* _litoa(long val, char* str, int base)
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

char* sitoa(size_t val, char* str)
{
    return ( _sitoa(val, str, 10) );
}

char* _sitoa(size_t val, char* str, int base)
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

char* ftoa(float val, char* str)
{
    const char digit[] = "0123456789";
    size_t pos = 0;
    
    // Get integer component.
    litoa((long)val, str);
    
    // Get fractional component.
    pos = strlen(str);
    str[pos] = '.';
    ++pos;
    
    val -= (long)val;
    for (size_t i = 0; i < 6; ++i)
    {
        val *= 10.0F;
        str[pos] = digit[(long)val % 10];
        val -= (long)val;
        ++pos;
    }
    
    str[pos] = '\0';

    return (str);
}

static unsigned long int rand_seed;
int rand_max(unsigned int max) // RAND_MAX assumed to be 32767
{
    static bool seeded = false;
    if (!seeded)
    {
        volatile unsigned long int start = cpu_rdtsc();
        volatile unsigned long int skip = start;
        while (skip /= 10)
        {
            cpu_io_wait();
            skip += (cpu_rdtsc() - start) % 10;
        }
        skip = cpu_rdtsc() - start;
        rand_seed = cpu_rdtsc() - start;
        seeded = true;
    }

    rand_seed = rand_seed * 1103515245 + 12345;
    return (unsigned int)(rand_seed / 65536) % (max + 1);
}

int rand()
{
    return rand_max(32767);
}

void srand( unsigned int seed )
{
    rand_seed = seed;
}
