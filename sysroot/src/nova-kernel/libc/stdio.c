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
static char stderr_buf[1000 + 2];
static FILE stderr_file;

void stdio_init(void)
{
    // Set up stdin.
    stdin_buf[1001] = '\0';
    stdin_file.buf = stdin_buf;
    stdin_file.pos = 0;
    stdin_file.len = 0;
    stdin_file.max_len = 1000;
    stdin_file.buf_mode = _IOLBF;
    stdin_file.io_mode = _IOI;
    stdin = &stdin_file;

    // Set up stdout.
    stdout_buf[10001] = '\0';
    stdout_file.buf = stdout_buf;
    stdout_file.pos = 0;
    stdout_file.len = 0;
    stdout_file.max_len = 10000;
    stdout_file.buf_mode = _IOLBF;
    stdout_file.io_mode = _IOO;
    stdout = &stdout_file;

    // Set up stderr.
    stderr_buf[1001] = '\0';
    stderr_file.buf = stderr_buf;
    stderr_file.pos = 0;
    stderr_file.len = 0;
    stderr_file.max_len = 1000;
    stderr_file.buf_mode = _IONBF;
    stderr_file.io_mode = _IOO;
    stderr = &stderr_file;
}

int fflush(FILE *stream)
{
    char* p = (char*) stream->buf;
    size_t pos = stream->pos;
    size_t len = stream->len;
    size_t max_len = stream->max_len;

    // No need to flush an empty stream.
    if (len == 0)
    {
        return (0);
    }

    // Print stdout/stderr while flushing.
    if (stream == stdout || stream == stderr)
    {
        // Check if the buffer wraps around.
        if (pos + len >= max_len)
        {
            size_t tmp_len = max_len - pos;

            kernel_write(&p[pos], tmp_len);
            len -= tmp_len;
        }

        kernel_write(&p[pos], len);
        len = 0;
        stream->len = len;
    }

    return (0);
}

int fputc(int c, FILE *stream)
{
    char* p = (char*) stream->buf;
    size_t pos = stream->pos;
    size_t len = stream->len;
    size_t max_len = stream->max_len;
    int buf_mode = stream->buf_mode;

    // Check if the stream is at maximum length.
    if (len == max_len)
    {
        return (EOF);
    }

    // Check if the buffer should wrap around.
    if (pos + len >= max_len - 1)
    {
        size_t tmp_len = len + pos - max_len;

        p[pos + tmp_len] = c;
        p[pos + tmp_len + 1] = '\0';
    }
    else
    {
        p[pos + len] = c;
        p[pos + len + 1] = '\0';
    }

    stream->len++;

    // Check if the buffer should be flushed.
    if (buf_mode == _IONBF)
    {
        fflush(stream);
    }
    else if (buf_mode == _IOLBF && (c == '\n' || len == max_len))
    {
        fflush(stream);
    }
    else if (buf_mode == _IOFBF && len == max_len)
    {
        fflush(stream);
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
            return (EOF);
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
    int ret;

    // Wait until stdin buffer is non-empty.
    if (stream == stdin)
    {
        while (stream->len == 0)
        {
            // Wait.
        }
    }

    // Check if stream is empty.
    if (stream->len == 0)
    {
        return (EOF);
    }

    // Get character.
    ret = stream->buf[stream->pos];
    stream->len--;
    stream->pos++;
    if (stream->pos == stream->max_len)
        stream->pos = 0;

    return (ret);
}

int getc(FILE *stream)
{
    return ( fgetc(stream) );
}

int getchar(void)
{
    int c = fgetc(stdin);
    putchar(c);
    fflush(stdout);
    return (c);
}

char* fgets(char *s, int n, FILE *stream)
{
    char c;

    c = fgetc(stream);
    if (c == EOF)
        return ((char*)EOF);
    for (size_t i = 0; i < n && c != 0; i++)
    {
        s[i] = c;
        c = fgetc(stream);
    }

    return (s);
}

char* gets(char *s)
{
    char c;

    c = fgetc(stdin);
    if (c == EOF)
        return ((char*)EOF);
    for (size_t i = 0; c != 0 && c != '\n'; i++)
    {
        putchar(c);
        fflush(stdout);
        s[i] = c;
        c = fgetc(stdin);
    }

    return (s);
}

