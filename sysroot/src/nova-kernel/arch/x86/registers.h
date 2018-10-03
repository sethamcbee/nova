/**
 * @file registers.h
 * @author Seth McBee
 * @date 2018-10-3
 * @brief Register storage for process switching.
 */

#pragma once

#include <globals.h>

/// Storage for registers.
typedef struct Registers
{
    // General purpose.
    uint32_t eax;
    uint32_t ebx;       // 4
    uint32_t ecx;       // 8
    uint32_t edx;       // 12
    uint32_t ebp;       // 16
    uint32_t esp;       // 20
    uint32_t esi;       // 24
    uint32_t edi;       // 28

    // Special registers.
    uint32_t eip;       // 32
    uint32_t flags;     // 36
    uint32_t ss;        // 40
    uint32_t cs;        // 44
} Registers;

static_assert(offsetof(Registers, esp) == 20, "");
static_assert(offsetof(Registers, eip) == 32, "");
