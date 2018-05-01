// Authors: Seth McBee
// Created: 2018-4-13
// Description: PS/2 interface.

#ifndef PS2_H
#define PS2_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// PS/2 controller ports.
#define PS2_CMD     0x64
#define PS2_DATA    0x60

// PS/2 controller commands (not device commands).
#define PS2_CFG_R           0x20
#define PS2_CFG_W           0x60
#define PS2_PORT1_ENABLE    0xAE
#define PS2_PORT1_DISABLE   0xAD
#define PS2_PORT2_ENABLE    0xA8
#define PS2_PORT2_DISABLE   0xA7

void ps2_send(uint8_t val, uint16_t port);
uint8_t ps2_receive(uint16_t port);

#endif // PS2_H
