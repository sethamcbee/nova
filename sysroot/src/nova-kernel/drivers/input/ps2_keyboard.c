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
    #include <arch/x86_64/devices/ps2.h>
#endif

#ifdef ARCH_X86
    #include <arch/x86/cpu.h>
    #include <arch/x86/devices/ps2.h>
#endif

// Flushes the keyboard buffer.
static void ps2_kb_flush(void);

static bool control;
static bool shift;
static bool alt;
static uint8_t code_last;

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
    switch (code)
    {
//// Ordinary keystrokes. ////

// Letters.

    case 0x1C:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('A', stdin);
        else
            fputc('a', stdin);
        break;

    case 0x32:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('B', stdin);
        else
            fputc('b', stdin);
        break;

    case 0x21:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('C', stdin);
        else
            fputc('c', stdin);
        break;

    case 0x23:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('D', stdin);
        else
            fputc('d', stdin);
        break;

    case 0x24:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('E', stdin);
        else
            fputc('e', stdin);
        break;

    case 0x2B:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('F', stdin);
        else
            fputc('f', stdin);
        break;

    case 0x34:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('G', stdin);
        else
            fputc('g', stdin);
        break;

    case 0x33:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('H', stdin);
        else
            fputc('h', stdin);
        break;

    case 0x43:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('I', stdin);
        else
            fputc('i', stdin);
        break;

    case 0x3B:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('J', stdin);
        else
            fputc('j', stdin);
        break;

    case 0x42:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('K', stdin);
        else
            fputc('k', stdin);
        break;

    case 0x4B:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('L', stdin);
        else
            fputc('l', stdin);
        break;

    case 0x3A:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('M', stdin);
        else
            fputc('m', stdin);
        break;

    case 0x31:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('N', stdin);
        else
            fputc('n', stdin);
        break;

    case 0x44:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('O', stdin);
        else
            fputc('o', stdin);
        break;

    case 0x4D:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('P', stdin);
        else
            fputc('p', stdin);
        break;

    case 0x15:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('Q', stdin);
        else
            fputc('q', stdin);
        break;

    case 0x2D:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('R', stdin);
        else
            fputc('r', stdin);
        break;

    case 0x1B:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('S', stdin);
        else
            fputc('s', stdin);
        break;

    case 0x2C:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('T', stdin);
        else
            fputc('t', stdin);
        break;

    case 0x3C:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('U', stdin);
        else
            fputc('u', stdin);
        break;

    case 0x2A:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('V', stdin);
        else
            fputc('v', stdin);
        break;

    case 0x1D:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('W', stdin);
        else
            fputc('w', stdin);
        break;

    case 0x22:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('X', stdin);
        else
            fputc('x', stdin);
        break;

    case 0x35:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('Y', stdin);
        else
            fputc('y', stdin);
        break;

    case 0x1A:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('Z', stdin);
        else
            fputc('z', stdin);
        break;

// Numbers.

    case 0x16:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('!', stdin);
        else
            fputc('1', stdin);
        break;

    case 0x1E:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('@', stdin);
        else
            fputc('2', stdin);
        break;

    case 0x26:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('#', stdin);
        else
            fputc('3', stdin);
        break;

    case 0x25:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('$', stdin);
        else
            fputc('4', stdin);
        break;

    case 0x2E:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('%', stdin);
        else
            fputc('5', stdin);
        break;

    case 0x36:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('^', stdin);
        else
            fputc('6', stdin);
        break;

    case 0x3D:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('&', stdin);
        else
            fputc('7', stdin);
        break;

    case 0x3E:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('*', stdin);
        else
            fputc('8', stdin);
        break;

    case 0x46:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('(', stdin);
        else
            fputc('9', stdin);
        break;

    case 0x45:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc(')', stdin);
        else
            fputc('0', stdin);
        break;

// Whitespace.

    // Space.
    case 0x29:
        if (code_last == 0xF0)
            break;
        fputc(' ', stdin);
        break;

    // Enter.
    case 0x5A:
        if (code_last == 0xF0)
            break;
        fputc('\n', stdin);
        break;

    // Tab.
    case 0x0D:
        if (code_last == 0xF0)
            break;
        fputc('\t', stdin);
        break;

// Punctuation and special characters.

    case 0x0E:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('~', stdin);
        else
            fputc('`', stdin);
        break;

    case 0x4E:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('_', stdin);
        else
            fputc('-', stdin);
        break;

    case 0x55:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('+', stdin);
        else
            fputc('=', stdin);
        break;

    case 0x54:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('{', stdin);
        else
            fputc('[', stdin);
        break;

    case 0x5B:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('}', stdin);
        else
            fputc(']', stdin);
        break;

    case 0x5D:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('|', stdin);
        else
            fputc('\\', stdin);
        break;

    case 0x4C:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc(':', stdin);
        else
            fputc(';', stdin);
        break;

    case 0x52:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('"', stdin);
        else
            fputc('\'', stdin);
        break;

    case 0x41:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('<', stdin);
        else
            fputc(',', stdin);
        break;

    case 0x49:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('>', stdin);
        else
            fputc('.', stdin);
        break;

    case 0x4A:
        if (code_last == 0xF0)
            break;
        if (shift)
            fputc('?', stdin);
        else
            fputc('/', stdin);

// Modifiers.

    // Left shift.
    case 0x12:
        if (code_last == 0xF0)
            shift = false;
        else
            shift = true;
        break;

    // Right shift.
    case 0x59:
        if (code_last == 0xF0)
            shift = false;
        else
            shift = true;
        break;

//// Special cases. ////

    // Key released.
    case 0xF0:
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

    code_last = code;
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
