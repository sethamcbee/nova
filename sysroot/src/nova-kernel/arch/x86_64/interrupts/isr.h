// Authors: Seth McBee
// Created: 2018-4-11
// Description: x86-64 ISR functions.

#ifndef ISR_H
#define ISR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Selector error codes.
#define ISR_ERROR_EXTERNAL_BIT      0
#define ISR_ERROR_EXTERNAL_MASK     1
#define ISR_ERROR_TABLE_BIT         1
#define ISR_ERROR_TABLE_MASK        3
#define ISR_ERROR_TABLE_GDT         0
#define ISR_ERROR_TABLE_IDT1        1
#define ISR_ERROR_TABLE_LDT         2
#define ISR_ERROR_TABLE_IDT2        3
#define ISR_ERROR_INDEX_BIT         4
#define ISR_ERROR_INDEX_MASK        0b1111111111111

// ISR entry points.
void isr_0(void);
void isr_1(void);
void isr_2(void);
void isr_3(void);
void isr_4(void);
void isr_5(void);
void isr_6(void);
void isr_7(void);
void isr_8(void);
void isr_9(void);
void isr_10(void);
void isr_11(void);
void isr_12(void);
void isr_13(void);
void isr_14(void);
void isr_15(void);
void isr_16(void);
void isr_17(void);
void isr_18(void);
void isr_19(void);
void isr_20(void);
void isr_21(void);
void isr_22(void);
void isr_23(void);
void isr_24(void);
void isr_25(void);
void isr_26(void);
void isr_27(void);
void isr_28(void);
void isr_29(void);
void isr_30(void);
void isr_31(void);
void isr_32(void);
void isr_33(void);
void isr_34(void);
void isr_35(void);
void isr_36(void);
void isr_37(void);
void isr_38(void);
void isr_39(void);
void isr_40(void);
void isr_41(void);
void isr_42(void);
void isr_43(void);
void isr_44(void);
void isr_45(void);
void isr_46(void);
void isr_47(void);
void isr_128(void);

// ISR C extensions.
void isr_13_ext(uint32_t error_code);
void isr_32_ext(void);
void isr_33_ext(void);
void isr_39_ext(void);

#endif // ISR_H
