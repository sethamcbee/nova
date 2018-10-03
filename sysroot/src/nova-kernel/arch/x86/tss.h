/**
 * @file tss.h
 * @author Seth McBee
 * @date 2018-10-3
 * @brief x86 TSS constants and declarations.
 */

#pragma once

#include <globals.h>

typedef struct Tss_Descriptor
{
    uint16_t limit_0;
    uint16_t base_0;
    uint8_t base_1;
    uint8_t accessed : 1;
    uint8_t rw : 1;
    uint8_t dc : 1;
    uint8_t ex : 1;
    uint8_t reserved_0 : 1;
    uint8_t privilege : 2;
    uint8_t present : 1;
    uint8_t limit_1 : 4;
    uint8_t reserved_1 : 1;
    uint8_t long_mode : 1;
    uint8_t size : 1;
    uint8_t granularity : 1;
    uint8_t base_2;
    uint32_t base_3;
    uint32_t reserved_2;
} __attribute__((packed)) Tss_Descriptor;

typedef struct Tss
{
    uint32_t esp0;
} __attribute__((packed)) Tss;

// Long mode TSS.
__attribute__((aligned(16))) volatile Tss tss;

// Must be called to set up the TSS.
void tss_init(void);

// Loads new state into TR, setting ring# to mask.
void tss_load(uint16_t mask);
