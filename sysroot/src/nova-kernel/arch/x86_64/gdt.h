/**
 * @file gdt.h
 * @author Seth McBee
 * @date 2017-10-17
 * @brief GDT constants and declarations.
 */

#pragma once

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING3 0b11

// GDT entry indices and constants.
#define GDT_NULL            0x0
#define GDT_KERNEL_CODE     0x08
#define GDT_KERNEL_DATA     0x10
#define GDT_USER_CODE       0x18
#define GDT_USER_DATA       0x20
#define GDT_TSS             0x28
#define GDT_ENTRY_COUNT     6
// Replace 0x?0 with 0x?3 for ring 3.
// Replace 0x?8 with 0x?B for ring 3.
// Alternatively, just OR with RING3.

/// GDT entry structure.
typedef struct Gdt_Entry
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
} __attribute__((packed)) Gdt_Entry;

/// GDT pointer structure.
typedef struct Gdt_Ptr
{
    uint16_t size;
    size_t base;
} __attribute__((packed)) Gdt_Ptr;

/// Initialize GDT.
void gdt_init(void);

/// GDT structure, declared in assembly file.
extern Gdt_Entry gdt_entry[GDT_ENTRY_COUNT] __attribute__((aligned(16)));

/// Pointer to GDT, loaded by LGDT.
extern Gdt_Ptr gdt_ptr;

#ifdef __cplusplus
}
#endif
