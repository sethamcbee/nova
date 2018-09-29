// Authors: Seth McBee
// Created: 2018-9-5
// Description: Compile-time and run-time assertions.

#pragma once

#include <stdlib.h>
#include <string.h>

#include <kernel.h>

// Compiler built-in.
#define static_assert _Static_assert

void kassert(const char* file, const char* func, long line, const char* exp);

#ifndef NDEBUG
#define assert(x) \
    ((x) ? 0 : kassert(__FILE__, __func__, __LINE__, #x))
#else
#define assert(ignore) ((void)0)
#endif // DEBUG
