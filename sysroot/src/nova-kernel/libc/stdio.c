// Authors: Seth McBee
// Created: 2018-4-27
// Description: STDIO.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>

#include <kernel.h>

// STUB. Eventually these should be implemented using the VFS.
static char stdin_buf[1000 + 2];
static FILE stdin_file;
static char stdout_buf[10000 + 2];
static FILE stdout_file;

void stdio_init(void)
{
    // Set up stdin.
    stdin_buf[0] = '\0';
    stdin_buf[1001] = '\0';
    stdin_file.buf = stdin_buf;
    stdin_file.pos = 0;
    stdin_file.len = 0;
    stdin_file.max_len = 1000;
    stdin_file.mode = _IOLBF;
    stdin = &stdin_file;

    // Set up stdout.
    stdout_buf[0] = '\0';
    stdout_buf[10001] = '\0';
    stdout_file.buf = stdout_buf;
    stdout_file.pos = 0;
    stdout_file.len = 0;
    stdout_file.max_len = 10000;
    stdout_file.mode = _IONBF;
    stdout = &stdout_file;
}

int fflush(FILE *stream)
{
    char* p = stream->buf;
    size_t pos = stream->pos;
    size_t len = stream->len;
    size_t max_len = stream->max_len;

    // Print stdout while flushing.
    if (stream == stdout)
    {
        // Check if the buffer wraps around.
        if (pos + len >= max_len)
        {
            size_t tmp_len = max_len - pos;

            kernel_write(&p[pos], tmp_len);
            pos = 0;
            len -= tmp_len;
        }

        kernel_write(&p[pos], len);
        len = 0;

        stream->pos = pos;
        stream->len = len;
    }

    return (0);
}

int fputc(int c, FILE *stream)
{
    char* p = stream->buf;
    size_t pos = stream->pos;
    size_t len = stream->len;
    size_t max_len = stream->max_len;
    int mode = stream->mode;

    // Check if the stream is at maximum length.
    if (len == max_len)
    {
        return (EOF);
    }

    // Check if the buffer should wrap around.
    if (pos + len >= max_len - 1)
    {
        size_t tmp_len = len + pos - max_len;

        p[tmp_len] = c;
        p[tmp_len] = '\0';
    }
    else
    {
        p[len] = c;
        p[len + 1] = '\0';
    }

    stream->len += 1;

    // Check if the buffer should be flushed.
    if (mode == _IONBF)
    {
        return (true);
    }
    else if (mode == _IOLBF && (c == '\n' || len == max_len))
    {
        return (true);
    }
    else if (mode == _IOFBF && len == max_len)
    {
        return (true);
    }

    return (0);
}

int putc(int c, FILE *stream)
{
    return ( fputc(c, stream) );
}

int putchar(int c)
{
    return ( fputc(c, stdout) );
}

int fputs(const char *s, FILE *stream)
{
    int err;
    size_t len = strlen(s);

    for (size_t i = 0; i < len; i++)
    {
        err = fputc(s[i], stream);

        if (err == EOF)
        {
            return (err);
        }
    }

    return (0);
};

int puts(const char *s)
{
    return ( fputs(s, stdout) );
}

int fgetc(FILE *stream)
{

}

int getc(FILE *stream)
{
    return ( fgetc(stream) );
}

int getchar(void)
{
    return ( fgetc(stdin) );
}

char* gets(char *s)
{

}

char* fgets(char *s, int n, FILE *stream);
