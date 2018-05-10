// Authors: Seth McBee
// Created: 2018-5-8
// Description: x86-64 TSS constants and declarations.

#ifndef TSS_H
#define TSS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed))
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
} Tss_Descriptor;

typedef struct __attribute__((packed))
{
    uint32_t reserved_0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved_1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved_2;
    uint16_t reserved_3;
    uint16_t iopb_offset;
} Tss;

// Long mode TSS.
__attribute__((aligned(16)))
volatile Tss tss;

// Must be called to set up the TSS.
void tss_init(void);

#endif // TSS_H
