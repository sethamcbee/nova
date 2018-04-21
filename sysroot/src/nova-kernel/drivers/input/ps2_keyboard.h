// Authors: Seth McBee
// Created: 2018-4-12
// Description: Generic PS/2 keyboard driver.

#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// PS/2 keyboard responses.
#define PS2_KB_ACK        0xFA
#define PS2_KB_ECHO       0xEE
#define PS2_KB_REDO       0xFE
#define PS2_KB_PASS       0xAA
#define PS2_KB_FAIL1      0xFD
#define PS2_KB_FAIL2      0xFC
#define PS2_KB_ERROR1     0x00
#define PS2_KB_ERROR2     0xFF

// PS/2 keyboard commands.
#define PS2_KB_LED        0xED
#define PS2_KB_SCAN       0xF0
#define PS2_KB_ID         0xF2
#define PS2_KB_REPEAT     0xF3
#define PS2_KB_ENABLE     0xF4
#define PS2_KB_DISABLE    0xF5
#define PS2_KB_DEFAULT    0xF6
#define PS2_KB_RESET      0xFF

// PS/2 keyboard LED bits.
#define PS2_KB_LED_SCROLL 0
#define PS2_KB_LED_NUMBER 1
#define PS2_KB_LED_CAPS   2

// PS/2 keyboard scancode data bytes.
#define PS2_KB_SCAN_GET   0
#define PS2_KB_SCAN1      1
#define PS2_KB_SCAN2      2
#define PS2_KB_SCAN3      3

// PS/2 keyboard typematic bits.
// TODO: Handle this.

void ps2_keyboard_initialize(void);

void ps2_keyboard_main(void);

void ps2_keyboard_handle(uint8_t code);

// Keyboard handler queue.
#define PS2_KB_QUEUE_MAX 32
uint8_t ps2_keyboard_queue[PS2_KB_QUEUE_MAX];
uint8_t ps2_keyboard_queue_count;

// Stores keyboard mapping.
char ps2_keyboard_table[256];

#endif // PS2_KEYBOARD_H