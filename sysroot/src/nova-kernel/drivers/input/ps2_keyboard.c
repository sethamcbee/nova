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

//#ifdef ARCH_X86_64
    #include <arch/x86_64/cpu.h>
    #include <arch/x86_64/pic.h>
    #include <arch/x86_64/ps2.h>
//#endif

static void ps2_kb_flush(void);

void ps2_keyboard_initialize(void)
{
    ps2_keyboard_queue = 0;
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
}

void ps2_keyboard_main(void)
{
    // Get all awaiting input.
    while (ps2_keyboard_queue > 0)
    {
        uint8_t c = ps2_receive(PS2_DATA);

        ps2_keyboard_handle(c);

        ps2_keyboard_queue--;
    }
}

void ps2_keyboard_handle(uint8_t code)
{
    char s[10];

    switch (code)
    {
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
