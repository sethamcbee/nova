// Authors: Seth McBee
// Created: 2018-4-12
// Description: Generic PS/2 keyboard driver.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <kernel.h>
#include <drivers/input/ps2_keyboard.h>

#ifdef ARCH_X86_64
    #include <arch/x86_64/cpu.h>
    #include <arch/x86_64/pic.h>
    #include <arch/x86_64/ps2.h>
#endif

#ifdef ARCH_X86
    #include <arch/x86/cpu.h>
    #include <arch/x86/pic.h>
    #include <arch/x86/ps2.h>
#endif

static void ps2_kb_flush(void);

void ps2_keyboard_initialize(void)
{
    ps2_keyboard_queue_count = 0;
    volatile uint8_t response = 0;

    // Disable 2nd PS/2 channel.
    ps2_send(0xA7, PS2_CMD);

    // Flush output buffer.
    ps2_kb_flush();

    // Disable port translation.
    do
    {
        ps2_send(PS2_CFG_R, PS2_CMD);
        response = ps2_receive(PS2_DATA);
    }
    while (response == PS2_KB_REDO);
    response = BIT_CLEAR(response, 6);
    ps2_send(PS2_CFG_W, PS2_CMD);
    ps2_send(response, PS2_DATA);

    // Set scan mode 2.
    do
    {
        ps2_send(PS2_KB_SCAN, PS2_DATA);
        response = ps2_receive(PS2_DATA);
    }
    while ((response != PS2_KB_ACK) && (response != PS2_KB_REDO));
    do
    {
        ps2_send(PS2_KB_SCAN2, PS2_DATA);
        response = ps2_receive(PS2_DATA);
    }
    while ((response != PS2_KB_ACK) && (response != PS2_KB_REDO));

    // Flush again.
    #if defined(ARCH_X86_64) || defined(ARCH_X86)
        for (int i = 0; i < 10; i++)
            cpu_io_wait();
        asm volatile ("sti \n");
    #endif
    ps2_kb_flush();
    #if defined(ARCH_X86_64) || defined(ARCH_X86)
        asm volatile ("cli \n");
    #endif
}

void ps2_keyboard_main(void)
{
    // Handle all awaiting input.
    for (size_t i = 0; i < ps2_keyboard_queue_count; i++)
    {
        ps2_keyboard_handle(ps2_keyboard_queue[i]);
    }
    ps2_keyboard_queue_count = 0;
}

void ps2_keyboard_handle(uint8_t code)
{
    char s[10];

    switch (code)
    {
//// Ordinary keystrokes. ////

//// Special cases. ////

    // Detection or buffer overrun error.
    case 0x00:
        kernel_log("PS/2 Keyboard: Received 0x00. Key detection or buffer overrun error.");
        break;

    // Test passed.
    case 0xAA:
        break;

    // Echo.
    case 0xEE:
        break;

    // ACK.
    case 0xFA:
        break;

    // Test failed.
    case 0xFC:
    case 0xFD:
        break;

    // Resend.
    case 0xFE:
        break;

    // Detection or buffer overrun error.
    case 0xFF:
        break;

    default:
        itoa(code, s, 16);
        strcat(s, "\n");
        kernel_print(s);
        break;
    }
}

static void ps2_kb_flush(void)
{
    bool ret = false;
    volatile uint8_t byte;
    do
    {
        cpu_io_wait();
        byte = cpu_inb(PS2_CMD);
        ret = BIT_CHECK(byte, 0);

        // Eat data.
        if (ret)
        {
            cpu_io_wait();
            byte = cpu_inb(PS2_DATA);
        }
    }
    while (ret);
}
