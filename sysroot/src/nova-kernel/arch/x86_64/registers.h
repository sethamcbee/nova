/**
 * @file registers.h
 * @author Seth McBee
 * @date 2018-9-6
 * @brief Register storage for process switching.
 */

#pragma once

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Storage for registers.
typedef struct Registers
{
    // General purpose.
    uint64_t rax;
    uint64_t rbx;       // 8
    uint64_t rcx;       // 16
    uint64_t rdx;       // 24
    uint64_t rbp;       // 32
    uint64_t rsp;       // 40
    uint64_t rsi;       // 48
    uint64_t rdi;       // 56
    uint64_t r8;        // 64
    uint64_t r9;        // 72
    uint64_t r10;       // 80
    uint64_t r11;       // 88
    uint64_t r12;       // 96
    uint64_t r13;       // 104
    uint64_t r14;       // 112
    uint64_t r15;       // 120

    // Special registers.
    uint64_t rip;       // 128
    uint64_t flags;     // 136
    uint64_t ss;        // 144
    uint64_t cs;        // 152
} Registers;

static_assert(offsetof(Registers, rsp) == 40, "");
static_assert(offsetof(Registers, rip) == 128, "");

#ifdef __cplusplus
}
#endif
