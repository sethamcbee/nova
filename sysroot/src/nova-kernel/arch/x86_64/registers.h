// Authors: Seth McBee
// Created: 2018-9-6
// Description: Register storage for process switching.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    // General purpose.
    uint64_t reg_rax;
    uint64_t reg_rbx;
    uint64_t reg_rcx;
    uint64_t reg_rdx;
    uint64_t reg_rbp;
    uint64_t reg_rsp;
    uint64_t reg_rsi;
    uint64_t reg_rdi;
    uint64_t reg_r8;
    uint64_t reg_r9;
    uint64_t reg_r10;
    uint64_t reg_r11;
    uint64_t reg_r12;
    uint64_t reg_r13;
    uint64_t reg_r14;
    uint64_t reg_r15;

    // Special registers.
    uint64_t reg_rip;
    uint64_t reg_flags;
} Registers;
