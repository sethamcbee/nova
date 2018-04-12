/**
 * @file ctype.h
 *
 * @author Seth Nils
 * @date 2014/6/5
 */

#ifndef _CTYPE_H_INCLUDED
#define _CTYPE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Returns true if isalpha(c) or idigit(c) is true.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isalnum(int c);

/**
 * @brief Returns true if isupper(c) or islower(c) is true.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isalpha(int c);

/**
 * @brief Returns true if character is a control character.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int iscntrl(int c);

/**
 * @brief Returns true if character is a decimal digit.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isdigit(int c);

/**
 * @brief Returns true if character is a printing character, not including
 * white space.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isgraph(int c);

/**
 * @brief Returns true if character is a lower-case letter.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int islower(int c);

/**
 * @brief Returns true if character is a printing character, including white
 * space.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isprint(int c);

/**
 * @brief Returns true if character is a printing character, not including
 * space or letter or digit.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int ispunct(int c);

/**
 * @brief Returns true if character is a space, linefeed, formfeed, newline,
 * carriage return, tab, vertical tab, or otherwise whitespace.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isspace(int c);

/**
 * @brief Returns true if character is an upper-case letter.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isupper(int c);

/**
 * @brief Returns true if character is a hexadecimal digit.
 *
 * @param c Character to check.
 *
 * @return 0 if false, non-0 if true.
 */
int isxdigit(int c);

/**
 * @brief Converts character to lower-case.
 *
 * @param c Character to check.
 *
 * @return If c is upper-case, return c converted to lower-case. Otherwise
 * return c.
 */
int tolower(int c);

/**
 * @brief Converts character to upper-case.
 *
 * @param c Character to check.
 *
 * @return If c is lower-case, return c converted to upper-case. Otherwise
 * return c.
 */
int toupper(int c);

#ifdef __cplusplus
}
#endif

#endif /* _CTYPE_H_INCLUDED */
