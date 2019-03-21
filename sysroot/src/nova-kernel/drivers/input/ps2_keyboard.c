/**
 * @file ps2_keyboard.c
 * @author Seth McBee
 * @date 2018-4-12
 * @brief Generic PS/2 keyboard driver.
 */

#include <globals.h>

#include <stdlib.h>
#include <string.h>

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

uint8_t ps2_keyboard_queue[PS2_KB_QUEUE_MAX];
uint8_t ps2_keyboard_queue_count;
char ps2_keyboard_table[256];
FILE* ps2_keyboard_stream;

// Flushes the keyboard buffer.
static void ps2_kb_flush(void);

static FILE ps2_keyboard_file;
static char ps2_keyboard_buf[100];

static bool control;
static bool shift;
static bool alt;
static uint8_t code_last;

void ps2_keyboard_init(void)
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

    // Set up stdio interface.
    ps2_keyboard_file.buf = ps2_keyboard_buf;
    ps2_keyboard_file.pos = 0;
    ps2_keyboard_file.len = 0;
    ps2_keyboard_file.max_len = 100;
    ps2_keyboard_file.buf_mode = _IONBF;
    ps2_keyboard_file.io_mode = _IOI;
    ps2_keyboard_file.read = read_null;
    ps2_keyboard_stream = &ps2_keyboard_file;
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
            rputc('A', ps2_keyboard_stream);
        else
            rputc('a', ps2_keyboard_stream);
        break;

    case 0x32:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('B', ps2_keyboard_stream);
        else
            rputc('b', ps2_keyboard_stream);
        break;

    case 0x21:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('C', ps2_keyboard_stream);
        else
            rputc('c', ps2_keyboard_stream);
        break;

    case 0x23:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('D', ps2_keyboard_stream);
        else
            rputc('d', ps2_keyboard_stream);
        break;

    case 0x24:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('E', ps2_keyboard_stream);
        else
            rputc('e', ps2_keyboard_stream);
        break;

    case 0x2B:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('F', ps2_keyboard_stream);
        else
            rputc('f', ps2_keyboard_stream);
        break;

    case 0x34:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('G', ps2_keyboard_stream);
        else
            rputc('g', ps2_keyboard_stream);
        break;

    case 0x33:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('H', ps2_keyboard_stream);
        else
            rputc('h', ps2_keyboard_stream);
        break;

    case 0x43:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('I', ps2_keyboard_stream);
        else
            rputc('i', ps2_keyboard_stream);
        break;

    case 0x3B:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('J', ps2_keyboard_stream);
        else
            rputc('j', ps2_keyboard_stream);
        break;

    case 0x42:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('K', ps2_keyboard_stream);
        else
            rputc('k', ps2_keyboard_stream);
        break;

    case 0x4B:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('L', ps2_keyboard_stream);
        else
            rputc('l', ps2_keyboard_stream);
        break;

    case 0x3A:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('M', ps2_keyboard_stream);
        else
            rputc('m', ps2_keyboard_stream);
        break;

    case 0x31:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('N', ps2_keyboard_stream);
        else
            rputc('n', ps2_keyboard_stream);
        break;

    case 0x44:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('O', ps2_keyboard_stream);
        else
            rputc('o', ps2_keyboard_stream);
        break;

    case 0x4D:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('P', ps2_keyboard_stream);
        else
            rputc('p', ps2_keyboard_stream);
        break;

    case 0x15:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('Q', ps2_keyboard_stream);
        else
            rputc('q', ps2_keyboard_stream);
        break;

    case 0x2D:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('R', ps2_keyboard_stream);
        else
            rputc('r', ps2_keyboard_stream);
        break;

    case 0x1B:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('S', ps2_keyboard_stream);
        else
            rputc('s', ps2_keyboard_stream);
        break;

    case 0x2C:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('T', ps2_keyboard_stream);
        else
            rputc('t', ps2_keyboard_stream);
        break;

    case 0x3C:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('U', ps2_keyboard_stream);
        else
            rputc('u', ps2_keyboard_stream);
        break;

    case 0x2A:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('V', ps2_keyboard_stream);
        else
            rputc('v', ps2_keyboard_stream);
        break;

    case 0x1D:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('W', ps2_keyboard_stream);
        else
            rputc('w', ps2_keyboard_stream);
        break;

    case 0x22:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('X', ps2_keyboard_stream);
        else
            rputc('x', ps2_keyboard_stream);
        break;

    case 0x35:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('Y', ps2_keyboard_stream);
        else
            rputc('y', ps2_keyboard_stream);
        break;

    case 0x1A:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('Z', ps2_keyboard_stream);
        else
            rputc('z', ps2_keyboard_stream);
        break;

// Numbers.

    case 0x16:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('!', ps2_keyboard_stream);
        else
            rputc('1', ps2_keyboard_stream);
        break;

    case 0x1E:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('@', ps2_keyboard_stream);
        else
            rputc('2', ps2_keyboard_stream);
        break;

    case 0x26:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('#', ps2_keyboard_stream);
        else
            rputc('3', ps2_keyboard_stream);
        break;

    case 0x25:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('$', ps2_keyboard_stream);
        else
            rputc('4', ps2_keyboard_stream);
        break;

    case 0x2E:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('%', ps2_keyboard_stream);
        else
            rputc('5', ps2_keyboard_stream);
        break;

    case 0x36:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('^', ps2_keyboard_stream);
        else
            rputc('6', ps2_keyboard_stream);
        break;

    case 0x3D:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('&', ps2_keyboard_stream);
        else
            rputc('7', ps2_keyboard_stream);
        break;

    case 0x3E:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('*', ps2_keyboard_stream);
        else
            rputc('8', ps2_keyboard_stream);
        break;

    case 0x46:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('(', ps2_keyboard_stream);
        else
            rputc('9', ps2_keyboard_stream);
        break;

    case 0x45:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc(')', ps2_keyboard_stream);
        else
            rputc('0', ps2_keyboard_stream);
        break;

// Whitespace.

    // Space.
    case 0x29:
        if (code_last == 0xF0)
            break;
        rputc(' ', ps2_keyboard_stream);
        break;

    // Enter.
    case 0x5A:
        if (code_last == 0xF0)
            break;
        rputc('\n', ps2_keyboard_stream);
        break;

    // Tab.
    case 0x0D:
        if (code_last == 0xF0)
            break;
        rputc('\t', ps2_keyboard_stream);
        break;

// Punctuation and special characters.

    case 0x0E:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('~', ps2_keyboard_stream);
        else
            rputc('`', ps2_keyboard_stream);
        break;

    case 0x4E:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('_', ps2_keyboard_stream);
        else
            rputc('-', ps2_keyboard_stream);
        break;

    case 0x55:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('+', ps2_keyboard_stream);
        else
            rputc('=', ps2_keyboard_stream);
        break;

    case 0x54:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('{', ps2_keyboard_stream);
        else
            rputc('[', ps2_keyboard_stream);
        break;

    case 0x5B:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('}', ps2_keyboard_stream);
        else
            rputc(']', ps2_keyboard_stream);
        break;

    case 0x5D:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('|', ps2_keyboard_stream);
        else
            rputc('\\', ps2_keyboard_stream);
        break;

    case 0x4C:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc(':', ps2_keyboard_stream);
        else
            rputc(';', ps2_keyboard_stream);
        break;

    case 0x52:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('"', ps2_keyboard_stream);
        else
            rputc('\'', ps2_keyboard_stream);
        break;

    case 0x41:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('<', ps2_keyboard_stream);
        else
            rputc(',', ps2_keyboard_stream);
        break;

    case 0x49:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('>', ps2_keyboard_stream);
        else
            rputc('.', ps2_keyboard_stream);
        break;

    case 0x4A:
        if (code_last == 0xF0)
            break;
        if (shift)
            rputc('?', ps2_keyboard_stream);
        else
            rputc('/', ps2_keyboard_stream);
        break;

// Modifiers.

    // Backspace.
    case 0x66:
        if (code_last == 0xF0)
            break;
        rputc('\b', ps2_keyboard_stream);
        break;

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

/// Eat garbage data from the device's internal buffer.
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
