/**
 * @file ctype.c
 * @author Seth McBee
 * @date 2014-?-?
 * @brief C standard library ctype implementation.
 */

#include <globals.h>

#include <ctype.h>

int isalnum(int c)
{
    if ((isalpha(c) != 0) || (isdigit(c) != 0))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isalpha(int c)
{
    if ((isupper(c) != 0) || (islower(c) != 0))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int iscntrl(int c)
{
    if ((c >= 0 && c <= 31) || (c == 127))
    {
        return (1);
    }
    else
    {
        return 0;
    }
}

int isdigit(int c)
{
    if (c >= 48 && c <= 57)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isgraph(int c)
{
    if ((isalnum(c) != 0) || (ispunct(c) != 0))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int islower(int c)
{
    if (c >= 97 && c <= 122)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isprint(int c)
{
    if ((isgraph(c) != 0) || (isspace(c) != 0))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int ispunct(int c)
{
    if
    (
        (c >= 33 && c <= 47)
        ||
        (c >= 58 && c <= 64)
        ||
        (c >= 91 && c <= 96)
        ||
        (c >= 123 && c <= 126)
    )
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isspace(int c)
{
    if ((c >= 9 && c <= 13) || (c == 32))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isupper(int c)
{
    if (c >= 65 && c <= 90)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int isxdigit(int c)
{
    if ((isdigit(c) != 0) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int tolower(int c)
{
    if (isupper(c) != 0)
    {
        /* Sets bit 6 of character, adding 32 to its decimal value and thus
         * converting from upper-case to lower-case. */
        return (BIT_SET(c, 6));
    }
    else
    {
        return (0);
    }
}

int toupper(int c)
{
    if (islower(c) != 0)
    {
        /* Clears bit 6 of character, subtracting 32 from its decimal value
         * and thus converting from lower-case to upper-case. */
        return (BIT_CLEAR(c, 6));
    }
    else
    {
        return (0);
    }
}
