// Authors: Seth McBee
// Created: 2018-4-12
// Description: Generic PS/2 keyboard driver.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <kernel.h>
#include <hal/keyboard.h>
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

static bool control;
static bool shift;
static bool alt;
static uint8_t code_last;
static bool ignore_next;
static bool release_code;

void ps2_keyboard_initialize(void)
{
    #if defined(ARCH_X86_64) || defined(ARCH_X86)
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
        ps2_kb_flush();
    #endif

    shift = false;
    control = false;
    alt = false;
    code_last = 0;
    ignore_next = false;
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
    if (ignore_next == true)
    {
        ignore_next = false;
        return;
    }

    switch (code)
    {
//// Ordinary keystrokes. ////

    case 0x1C:
        if (shift)
            fputc('A', stdin);
        else
            fputc('a', stdin);
        break;

    case 0x32:
        if (shift)
            fputc('B', stdin);
        else
            fputc('b', stdin);
        break;

    case 0x21:
        if (shift)
            fputc('C', stdin);
        else
            fputc('c', stdin);
        break;

    case 0x23:
        if (shift)
            fputc('D', stdin);
        else
            fputc('d', stdin);
        break;

    case 0x24:
        if (shift)
            fputc('E', stdin);
        else
            fputc('e', stdin);
        break;

    case 0x2B:
        if (shift)
            fputc('F', stdin);
        else
            fputc('f', stdin);
        break;

    case 0x34:
        if (shift)
            fputc('G', stdin);
        else
            fputc('g', stdin);
        break;

    case 0x33:
        if (shift)
            fputc('H', stdin);
        else
            fputc('h', stdin);
        break;

    case 0x43:
        if (shift)
            fputc('I', stdin);
        else
            fputc('i', stdin);
        break;

    case 0x3B:
        if (shift)
            fputc('J', stdin);
        else
            fputc('j', stdin);
        break;

    case 0x42:
        if (shift)
            fputc('K', stdin);
        else
            fputc('k', stdin);
        break;

    case 0x4B:
        if (shift)
            fputc('L', stdin);
        else
            fputc('l', stdin);
        break;

    case 0x3A:
        if (shift)
            fputc('M', stdin);
        else
            fputc('m', stdin);
        break;

    case 0x31:
        if (shift)
            fputc('N', stdin);
        else
            fputc('n', stdin);
        break;

    case 0x44:
        if (shift)
            fputc('O', stdin);
        else
            fputc('o', stdin);
        break;

    case 0x4D:
        if (shift)
            fputc('P', stdin);
        else
            fputc('p', stdin);
        break;

    case 0x15:
        if (shift)
            fputc('Q', stdin);
        else
            fputc('q', stdin);
        break;

    case 0x2D:
        if (shift)
            fputc('R', stdin);
        else
            fputc('r', stdin);
        break;

    case 0x1B:
        if (shift)
            fputc('S', stdin);
        else
            fputc('s', stdin);
        break;

    case 0x2C:
        if (shift)
            fputc('T', stdin);
        else
            fputc('t', stdin);
        break;

    case 0x3C:
        if (shift)
            fputc('U', stdin);
        else
            fputc('u', stdin);
        break;

    case 0x2A:
        if (shift)
            fputc('V', stdin);
        else
            fputc('v', stdin);
        break;

    case 0x1D:
        if (shift)
            fputc('W', stdin);
        else
            fputc('w', stdin);
        break;

    case 0x22:
        if (shift)
            fputc('X', stdin);
        else
            fputc('x', stdin);
        break;

    case 0x35:
        if (shift)
            fputc('Y', stdin);
        else
            fputc('y', stdin);
        break;

    case 0x1A:
        if (shift)
            fputc('Z', stdin);
        else
            fputc('z', stdin);
        break;

    case 0x29:
        fputc(' ', stdin);
        break;

    case 0x5A:
        fputc('\n', stdin);
        break;

//// Special cases. ////

    // Key released.
    case 0xF0:
        ignore_next = true;
        break;

//// Command codes. ////

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
