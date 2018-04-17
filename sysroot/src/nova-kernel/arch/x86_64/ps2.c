// Authors: Seth McBee
// Created: 2018-4-13
// Description: PS/2 interface.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/ps2.h>

// Helper functions. Wait until the controller is ready for I/O.
static void ps2_ready_send(void);
static void ps2_ready_receive(void);

void ps2_send(uint8_t val, uint16_t port)
{
    ps2_ready_send();
    cpu_io_wait();
    cpu_outb(val, port);
}

uint8_t ps2_receive(uint16_t port)
{
    ps2_ready_receive();
    cpu_io_wait();
    return (cpu_inb(port));
}

static void ps2_ready_send(void)
{
    bool ret = false;
    uint8_t byte;
    while (!ret)
    {
        cpu_io_wait();
        byte = cpu_inb(PS2_CMD);
        ret = ~BIT_CHECK(byte, 1);
    }
}

static void ps2_ready_receive(void)
{
    bool ret = false;
    uint8_t byte;
    while (!ret)
    {
        cpu_io_wait();
        byte = cpu_inb(PS2_CMD);
        ret = BIT_CHECK(byte, 0);
    }
}
