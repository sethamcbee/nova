// Authors: Seth McBee
// Created: 2018-9-6
// Description: Register storage for process switching.

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef struct
{
    // General purpose.
    uint64_t reg_rax;
    uint64_t reg_rbx;       // 8
    uint64_t reg_rcx;       // 16
    uint64_t reg_rdx;       // 24
    uint64_t reg_rbp;       // 32
    uint64_t reg_rsp;       // 40
    uint64_t reg_rsi;       // 48
    uint64_t reg_rdi;       // 56
    uint64_t reg_r8;        // 64
    uint64_t reg_r9;        // 72
    uint64_t reg_r10;       // 80
    uint64_t reg_r11;       // 88
    uint64_t reg_r12;       // 96
    uint64_t reg_r13;       // 104
    uint64_t reg_r14;       // 112
    uint64_t reg_r15;       // 120

    // Special registers.
    uint64_t reg_rip;       // 128
    uint64_t reg_flags;     // 136
    uint64_t reg_ss;        // 144
    uint64_t reg_cs;        // 152
} Registers;

static_assert(offsetof(Registers, reg_rsp) == 40, "");
static_assert(offsetof(Registers, reg_rip) == 128, "");
