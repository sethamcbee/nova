// Authors: Seth McBee
// Created: 2018-5-3
// Description: Generic terminal emulator driver.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <kernel.h>
#include <drivers/input/ps2_keyboard.h>
#include <hal/tty.h>

static FILE tty_out_file;
static char tty_out_buf[10000];
static FILE tty_in_file;
static char tty_in_buf[10000];
static char tty_in_str[10000 + 1];
static size_t tty_in_len;
static ssize_t (*ext_write)(const void *, size_t n);
//static ssize_t (*ext_read)(void *, size_t n);

// Writes data.
static ssize_t tty_write(const void *s, size_t n)
{
    return ( ext_write(s, n) );
}

// Reads data.
static ssize_t tty_read(void *s, size_t n)
{
    char c;
    char *str = s;
    size_t len = 0;

    // Flush stdout if the input stream is stdin.
    if (tty_ins == stdin)
    {
        fflush(stdout);
    }

    // Check if the stream still holds data.
    if (tty_ins->len > 0)
    {
        c = rgetc(tty_ins);
        for (len = 0; len < n && c != EOF; len++)
        {
            str[len] = c;

            if (c == '\n')
            {
                break;
            }

            c = rgetc(tty_ins);
        }

        if (len == n && c != EOF)
            rungetc(c, tty_ins);

        return (len);
    }

    // Handle any awaiting keyboard data.
    while (ps2_keyboard_stream->len == 0)
        ; // Wait.
    c = rgetc(ps2_keyboard_stream);
    while (tty_in_len < tty_in_file.max_len)
    {
        // Handle EOF.
        if (c == EOF)
        {
            while (ps2_keyboard_stream->len == 0)
                ; // Wait.
            c = rgetc(ps2_keyboard_stream);
            continue;
        }

        // Handle backspace.
        if (c == '\b')
        {
            // There must be something to delete.
            if (tty_in_len > 0)
            {
                tty_in_len--;
                if (tty_in_str[tty_in_len] == '\t')
                {
                    char tmp[] = "\b\b\b\b    \b\b\b\b";
                    size_t tmp_len = strlen(tmp);
                    fputn(tmp, tmp_len, tty_outs);
                }
                else
                {
                    char tmp[] = "\b \b";
                    size_t tmp_len = strlen(tmp);
                    fputn(tmp, tmp_len, tty_outs);
                }
                fflush(tty_outs);
            }
        }
        else
        {
            tty_in_str[tty_in_len] = c;
            fputc(c, tty_outs);
            fflush(tty_outs);
            tty_in_len++;
        }

        // Keyboard is newline delimitted.
        if (c == '\n')
        {
            break;
        }

        // Handle any awaiting keyboard data.
        while (ps2_keyboard_stream->len == 0)
            ; // Wait.
        c = rgetc(ps2_keyboard_stream);
    }

    // Copy data to stream.
    for (size_t i = 0; i < tty_in_len; i++)
    {
        c = tty_in_str[i];
        rputc(c, tty_ins);
    }
    tty_in_len = 0;

    // Pass data.
    c = rgetc(tty_ins);
    for (len = 0; len < n && c != EOF; len++)
    {
        str[len] = c;

        if (c == '\n')
        {
            break;
        }

        c = rgetc(tty_ins);
    }

    if (len == n)
        rungetc(c, tty_ins);

    return (len);
}

void tty_init(void)
{
    // Set up stdio interface.
    tty_out_file.buf = tty_out_buf;
    tty_out_file.pos = 0;
    tty_out_file.len = 0;
    tty_out_file.max_len = 10000;
    tty_out_file.buf_mode = _IOLBF;
    tty_out_file.io_mode = _IOO;
    tty_out_file.write = tty_write;
    tty_outs = &tty_out_file;

    tty_in_file.buf = tty_in_buf;
    tty_in_file.pos = 0;
    tty_in_file.len = 0;
    tty_in_file.max_len = 10000;
    tty_in_file.buf_mode = _IOLBF;
    tty_in_file.io_mode = _IOO;
    tty_in_file.read = tty_read;
    tty_ins = &tty_in_file;

    tty_in_len = 0;
    ext_write = kernel_write;
}
