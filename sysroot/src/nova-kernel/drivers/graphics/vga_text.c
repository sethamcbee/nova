// Authors: Seth McBee
// Created: 2018-10-14
// Description: VGA color text mode driver.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/graphics/vga_text.h>

static uint16_t *vga_buffer;
static uint16_t vga_cur_x;
static uint16_t vga_cur_y;
static uint16_t vga_color;

int vga_text_write(const void *str, size_t len)
{
    // Initialize terminal if it hasn't already been initialized.
    if (vga_text_initialized == false)
    {
        vga_text_initialize();
    }

    // Cast str as an char*.
    const char *p = str;

    // Write each character.
    for (size_t i = 0; i < len; i++)
    {
        switch (p[i])
        {
        // Just write the character.
        default:
            vga_text_put_char(p[i], vga_cur_x, vga_cur_y, vga_color);
            break;
        }

        // Move cursor to next position.
        vga_cur_x++;

        if (vga_cur_x == VGA_TEXT_WIDTH)
        {
            vga_cur_x = 0;
            vga_cur_y++;
        }
        if (vga_cur_y == VGA_TEXT_HEIGHT)
        {
            // Copy all entries up a row.
            for (size_t k = 0; k < VGA_TEXT_WIDTH*(VGA_TEXT_HEIGHT - 1); k++)
            {
                vga_buffer[k] = vga_buffer[k + VGA_TEXT_WIDTH];
            }

            // Decrement row.
            vga_cur_y--;

            // Clear bottom row.
            for (size_t k = 0; k < VGA_TEXT_WIDTH; k++)
            {
                vga_text_put_char(' ', k, VGA_TEXT_HEIGHT - 1, vga_color);
            }
        }
    }

    return (len);
}

void vga_text_initialize(void)
{
    vga_cur_x = 0;
    vga_cur_y = 0;
    vga_buffer = (uint16_t*) VGA_TEXT_BUFFER;
    vga_color = vga_text_make_color(LIGHT_GREY, BLACK);
    vga_text_initialized = true;

    // Clear the buffer.
    for (size_t i = 0; i < (VGA_TEXT_HEIGHT * VGA_TEXT_WIDTH); i++)
    {
    }
}

uint8_t vga_text_make_color(Vga_Text_Color fg, Vga_Text_Color bg)
{
    uint8_t color = fg | (bg << 4);
    return (color);
}

void vga_text_put_char(uint8_t c, uint8_t x, uint8_t y, uint16_t color)
{
    // Find position in the buffer.
    size_t pos = x + (y * VGA_TEXT_WIDTH);

    vga_buffer[pos] = c | (color << 8);
}
