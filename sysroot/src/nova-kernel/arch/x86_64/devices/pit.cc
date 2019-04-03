/**
 * @file pit.cc
 * @author Seth McBee
 * @date 2019-4-3
 */

#include <globals.h>

#include <stdio.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/devices/pit.h>

// TODO: Make these constexpr when the full project is rewritten in C++.
uint32_t PIT_BASE_FREQ = 1193182;
uint32_t PIT_TARGET_FREQ = 100;
uint16_t PIT_DIVISOR;
float PIT_REAL_FREQ;

// IO ports.
static constexpr uint16_t DATA_PORT = 0x40;
static constexpr uint16_t MODE_PORT = 0x43;

// Command word layout.
static constexpr uint8_t CHANNEL_BIT = 6;
static constexpr uint8_t ACCESS_MODE_BIT = 4;
static constexpr uint8_t OPERATING_MODE_BIT = 1;
static constexpr uint8_t BINARY_BCD_BIT = 0;

// Channel selection masks.
static constexpr uint8_t CHANNEL_0 = 0;
static constexpr uint8_t CHANNEL_1 = 1;
static constexpr uint8_t CHANNEL_2 = 2;
static constexpr uint8_t READ_BACK = 3;

// Access mode masks.
static constexpr uint8_t LATCH_COUNT = 0;
static constexpr uint8_t ACCESS_LOW = 1;
static constexpr uint8_t ACCESS_HIGH = 2;
static constexpr uint8_t ACCESS_FULL = 3;

// Operating mode masks.
static constexpr uint8_t RATE_GENERATOR = 2;
static constexpr uint8_t SQUARE_WAVE = 3;

void pit_init()
{
    PIT_DIVISOR = PIT_BASE_FREQ / PIT_TARGET_FREQ;
    PIT_REAL_FREQ = (float)PIT_BASE_FREQ / (float)PIT_DIVISOR;
    irq_pit_count = 0;
    
    assert(PIT_DIVISOR > 1);
    assert(PIT_REAL_FREQ <= PIT_BASE_FREQ);
    
    uint8_t low = (uint8_t)PIT_DIVISOR;
    uint8_t high = (uint8_t)(PIT_DIVISOR >> 8);
    
    // Set mode.
    uint8_t mode_cmd = SQUARE_WAVE << OPERATING_MODE_BIT;
    mode_cmd |= ACCESS_FULL << ACCESS_MODE_BIT;
    mode_cmd |= CHANNEL_0 << CHANNEL_BIT;
    cpu_outb(mode_cmd, MODE_PORT);
    cpu_io_wait();
    
    // Set divider.
    cpu_outb(low, DATA_PORT);
    cpu_io_wait();
    cpu_outb(high, DATA_PORT);
    cpu_io_wait();
}

void pit_info()
{
    printf("PIT base freq: %d Hz\n", PIT_BASE_FREQ);
    printf("PIT target freq: %d Hz\n", PIT_TARGET_FREQ);
    printf("PIT divisor: %d\n", PIT_DIVISOR);
    printf("PIT real freq: %f Hz\n", PIT_REAL_FREQ);
    printf("PIT real period: %f ms\n", 1000.0f/PIT_REAL_FREQ);
}
