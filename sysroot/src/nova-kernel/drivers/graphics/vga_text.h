// Authors: Seth McBee
// Created: 2018-10-14
// Description: VGA color text mode driver.

#ifndef VGA_TEXT_H
#define VGA_TEXT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_TEXT_BUFFER 0xB8000
#define VGA_TEXT_WIDTH 80
#define VGA_TEXT_HEIGHT 25

// Hardware color constants.
typedef enum Vga_Text_Color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
} Vga_Text_Color;

// Indicates whether the terminal has been initialized.
bool vga_text_initialized;

// Writes a string to the terminal, possibly returning an error code.
int vga_text_write(const void *str, size_t len);

// Initializes the driver before use.
void vga_text_initialize(void);

// Writes a single character at the given (x,y) with a certain color scheme.
void vga_text_put_char(uint8_t c, uint8_t x, uint8_t y, uint8_t color);

// Formats a color scheme.
uint8_t vga_text_make_color(Vga_Text_Color fg, Vga_Text_Color bg);

#endif // VGA_TEXT_H
