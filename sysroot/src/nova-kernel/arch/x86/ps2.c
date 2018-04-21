// Authors: Seth McBee
// Created: 2018-4-13
// Description: PS/2 interface.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>
#include <arch/x86/cpu.h>
#include <arch/x86/ps2.h>

// Helper functions. Waits until the controller is ready for I/O.
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

    uint64_t timer_start = cpu_rdtsc();
    while (!ret)
    {
        cpu_io_wait();
        byte = cpu_inb(PS2_CMD);
        ret = ~BIT_CHECK(byte, 1);

        // Check for timeout.
        if (cpu_rdtsc() - timer_start > 100000)
        {
            break;
        }
    }
}

static void ps2_ready_receive(void)
{
    bool ret = false;
    uint8_t byte;

    uint64_t timer_start = cpu_rdtsc();
    while (!ret)
    {
        cpu_io_wait();
        byte = cpu_inb(PS2_CMD);
        ret = BIT_CHECK(byte, 0);

        // Check for timeout.
        if (cpu_rdtsc() - timer_start > 100000)
        {
            break;
        }
    }
}
