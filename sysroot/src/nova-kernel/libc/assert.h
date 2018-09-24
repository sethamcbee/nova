// Authors: Seth McBee
// Created: 2018-9-5
// Description: Compile-time and run-time assertions.

#pragma once

#include <stdlib.h>
#include <string.h>

#include <kernel.h>

// Compiler built-in.
#define static_assert _Static_assert

__attribute((noreturn))
static void kassert(const char* file, const char* func, long line, const char* exp)
{
    char msg[500];
    char line_str[10];

    #if defined(ARCH_X86_64) || defined(ARCH_X86)
    //asm volatile ("cli \n");
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
}

#ifndef NDEBUG
#define assert(x) \
    ((x) ? 0 : kassert(__FILE__, __func__, __LINE__, #x))
#else
#define assert(ignore) ((void)0)
#endif // DEBUG
