/**
 * @file assert.c
 * @author Seth McBee
 * @date 2018-9-28
 * @brief Implementation of kernel-level assertions.
 */

#include <assert.h>

#include <kernel.h>

bool kassert(const char* file, const char* func, long line, const char* exp)
{
    char msg[500];
    char line_str[10];

#if defined(ARCH_X86_64) || defined(ARCH_X86)
    asm volatile ("cli \n");
#endif

    // Convert line number to a string.
    litoa(line, line_str);

    // Build output string.
    strcpy(msg, "Assertion failed:\n\nFile: ");
    strcat(msg, file);
    strcat(msg, "\nFunction: ");
    strcat(msg, func);
    strcat(msg, "\nLine: ");
    strcat(msg, line_str);
    strcat(msg, "\nExpression: ");
    strcat(msg, exp);
    strcat(msg, "\n");

    // Dispatch message.
    kernel_panic(msg);
    
    // Dead code.
    return false;
}
