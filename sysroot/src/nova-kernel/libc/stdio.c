// Authors: Seth McBee
// Created: 2018-4-27
// Description: STDIO.

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel.h>

// printf() specifiers.
#define WIDTH_ARG           -2
#define PRECISION_ARG       -2
#define LENGTH_SHORT        -1
#define LENGTH_DEFAULT      0
#define LENGTH_LONG         1
#define LENGTH_WIDE         1
#define LENGTH_DOUBLE       2

// STUB. Eventually these should be implemented using the VFS.
static char stdin_buf[1000];
static FILE stdin_file;
static char stdout_buf[10000];
static FILE stdout_file;
static char stderr_buf[1000];
static FILE stderr_file;

int rgetc(FILE *stream)
{
    int ret;

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

int rputc(int c, FILE *stream)
{
    // Not thread-safe (needs a per-stream lock, maybe).
    char* buf = (char*) stream->buf;
    size_t pos = stream->pos;
    size_t len = stream->len;
    size_t max_len = stream->max_len;

    // Check if the stream is at maximum length.
    if (stream->len == stream->max_len)
    {
        return (EOF);
    }

    // Check if the buffer should wrap around.
    if (pos + len >= max_len)
    {
        size_t tmp_len = len + pos - max_len;
        buf[pos + tmp_len] = c;
    }
    else
    {
        buf[pos + len] = c;
    }

    stream->len++;

    return (0);
}

int rungetc(int c, FILE *stream)
{
    // Not thread-safe (needs a per-stream lock, maybe).

    rputc(c, stream);

    // Move back position.
    stream->pos--;
    if (stream->pos == -1)
        stream->pos = stream->max_len;

    return (0);
}

void stdio_init(void)
{
    // Set up stdin.
    stdin_file.buf = stdin_buf;
    stdin_file.pos = 0;
    stdin_file.len = 0;
    stdin_file.max_len = 1000;
    stdin_file.buf_mode = _IOLBF;
    stdin_file.io_mode = _IOI;
    stdin_file.write = write_null;
    stdin = &stdin_file;

    // Set up stdout.
    stdout_file.buf = stdout_buf;
    stdout_file.pos = 0;
    stdout_file.len = 0;
    stdout_file.max_len = 10000;
    stdout_file.buf_mode = _IOLBF;
    stdout_file.io_mode = _IOO;
    stdout_file.write = write_null;
    stdout = &stdout_file;

    // Set up stderr.
    stderr_file.buf = stderr_buf;
    stderr_file.pos = 0;
    stderr_file.len = 0;
    stderr_file.max_len = 1000;
    stderr_file.buf_mode = _IONBF;
    stderr_file.io_mode = _IOO;
    stderr_file.write = write_null;
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

    // Do nothing with virtual string stream.
    if (stream->io_mode == _IOS)
    {
        return (0);
    }

    // Check for input-only stream.
    if (stream->io_mode == _IOI)
    {
        return (EOF);
    }

    // Check if the buffer wraps around.
    if (pos + len >= max_len)
    {
        size_t tmp_len = max_len - pos;

        stream->write(&p[pos], tmp_len);
        len -= tmp_len;
    }

    stream->write(&p[pos], len);
    len = 0;
    stream->len = len;

    return (0);
}

void clearerr(FILE *stream)
{

}

int fputn(const char *s, size_t n, FILE *stream)
{
    int ret = EOF;

    for (size_t i = 0; i < n; i++)
    {
        ret = fputc(s[i], stream);

        if (ret == EOF)
            return (EOF);
    }

    return (ret);
}

int fputc(int c, FILE *stream)
{
    // Write character.
    rputc(c, stream);

    // Check if the buffer should be flushed.
    if (stream->buf_mode == _IONBF)
    {
        fflush(stream);
    }
    else if (stream->buf_mode == _IOLBF && (c == '\n' || stream->len == stream->max_len))
    {
        fflush(stream);
    }
    else if (stream->buf_mode == _IOFBF && stream->len == stream->max_len)
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
    size_t len = strlen(s);

    return (fputn(s, len, stream));
};

int puts(const char *s)
{
    fputs(s, stdout);
    return ( fputc('\n', stdout) );
}

int fgetc(FILE *stream)
{
    int ret;

    // Get character.
    stream->read(&ret, 1);

    return (ret);
}

int getc(FILE *stream)
{
    return ( fgetc(stream) );
}

int ungetc(int c, FILE *stream)
{
    int ret;
    ret = rungetc(c, stream);
    return (ret);
}

int getchar(void)
{
    return ( fgetc(stdin) );
}

char* fgets(char *s, int n, FILE *stream)
{
    int len;
    len = stream->read(s, n);

    // Null terminate stream.
    s[len + 1] = '\0';

    return (s);
}

char* gets(char *s)
{
    int len;

    // Get line.
    fgets(s, INT_MAX, stdin);

    // Remove trailing newline, if any.
    len = strlen(s);
    if (s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
    }

    return (s);
}

int vfprintf(FILE *stream, const char *format, va_list arg)
{
    ssize_t len = 0;
    int written = 0;

    // Tags.
    char spec;
    bool left_justify = false;
    bool force_sign = false;
    bool force_space = false;
    bool force_spec = false;
    bool zero_pad = false;
    int width = -1;
    int precision = -1;
    int length = LENGTH_DEFAULT;

    while (format[len] != '\0')
    {
        if (format[len] == '%')
        {
            // Print string so far.
            fputn(format, len, stream);
            written += len;
            format += len + 1;
            len = -1;

            // Get tags.
            bool done = false;
            bool dot = false;
            while (!done)
            {
                switch (*format)
                {
                case '-':
                    left_justify = true;
                    break;

                case '+':
                    force_sign = true;
                    break;

                case ' ':
                    force_space = true;
                    break;

                case '#':
                    force_spec = true;
                    break;

                case '0':
                    zero_pad = true;
                    break;

                case '.':
                    dot = true;
                    break;

                case '*':
                    if (dot == true)
                    {
                        dot = false;
                        precision = PRECISION_ARG;
                    }
                    else
                        width = WIDTH_ARG;
                    break;

                case 'h':
                    length = LENGTH_SHORT;
                    break;

                case 'l':
                    length = LENGTH_LONG;
                    break;

                case 'L':
                    length = LENGTH_DOUBLE;
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (dot == true)
                    {
                        dot = false;
                        precision = atoi(format);
                    }
                    else
                    {
                        width = atoi(format);
                    }
                    while (isdigit(*format))
                        format++;
                    format--;
                    break;

                default:
                    done = true;
                    format--;
                    break;
                }
                format++;
            }

            // Get specifier.
            spec = *format;
            format++;

            // Character.
            if (spec == 'c' || spec == '%')
            {
                if (length != LENGTH_WIDE)
                {
                    char c = (char) va_arg(arg, int);
                    fputc(c, stream);
                    written++;
                }
                continue;
            }

            // String.
            if (spec == 's')
            {
                // Get precision from va_arg if specified.
                if (precision == PRECISION_ARG)
                    precision = (int) va_arg(arg, int);

                // Get pointer to string.
                const char *s = (const char*) va_arg(arg, const char *);

                if (precision > 0)
                {
                    fputn(s, precision, stream);
                    written += precision;
                }
                else
                {
                    fputs(s, stream);
                    written += strlen(s);
                }
                continue;
            }

            // Signed decimal integer.
            if (spec == 'd' || spec == 'i')
            {
                // Get width from va_arg if specified.
                if (width == WIDTH_ARG)
                    width = (int) va_arg(arg, int);

                char tmp[100 + width];
                long n;
                int pad;

                // Get integer.
                if (length == LENGTH_SHORT)
                    n = (short) va_arg(arg, int);
                else if (length == LENGTH_LONG)
                    n = (long) va_arg(arg, long);
                else
                    n = (int) va_arg(arg, int);

                // Format string.
                if (force_sign == true)
                {
                    if (n < 0)
                        tmp[0] = '-';
                    else
                        tmp[0] = '+';
                    litoa(labs(n), tmp+1);
                }
                else if (force_space == true)
                {
                    if (n < 0)
                        tmp[0] = '-';
                    else
                        tmp[0] = ' ';
                    litoa(labs(n), tmp+1);
                }
                else
                {
                    litoa(n, tmp);
                }

                fputs(tmp, stream);
                written += strlen(tmp);
                continue;
            }

            // Reset tags.
            left_justify = false;
            force_sign = false;
            force_space = false;
            force_spec = false;
            zero_pad = false;
            width = -1;
            precision = -1;
            length = LENGTH_DEFAULT;
        }

        len++;
    }

    // Print anything left.
    fputs(format, stream);

    return (written);
}

int fprintf(FILE *stream, const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vfprintf(stream, format, arg);
    va_end(arg);
    return (ret);
}

int printf(const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vfprintf(stdout, format, arg);
    return (ret);
}

int vprintf(const char *format, va_list arg)
{
    return ( vfprintf(stdout, format, arg) );
}

int vsprintf(char *s, const char *format, va_list arg)
{
    int ret;
    FILE tmp;
    FILE *stream = &tmp;
    tmp.buf = s;
    tmp.pos = 0;
    tmp.len = 0;
    tmp.max_len = SIZE_MAX;
    tmp.buf_mode = _IOFBF;
    tmp.io_mode = _IOS;
    tmp.write = &write_null;

    ret = vfprintf(stream, format, arg);
    return (ret);
}

int sprintf(char *s, const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vsprintf(s, format, arg);
    return (ret);
}

int vfscanf(FILE *stream, const char *format, va_list arg)
{
    ssize_t len = 0;
    int args_read = 0;

    // Tags.
    char spec;
    bool left_justify = false;
    bool force_sign = false;
    bool force_space = false;
    bool force_spec = false;
    bool zero_pad = false;
    int width = -1;
    int precision = -1;
    int length = LENGTH_DEFAULT;

    while (format[len] != '\0')
    {
        if (format[len] == '%')
        {
            // Move pointer to current position.
            format += len + 1;
            len = -1;

            // Get tags.
            bool done = false;
            bool dot = false;
            while (!done)
            {
                switch (*format)
                {
                case '-':
                    left_justify = true;
                    break;

                case '+':
                    force_sign = true;
                    break;

                case ' ':
                    force_space = true;
                    break;

                case '#':
                    force_spec = true;
                    break;

                case '0':
                    zero_pad = true;
                    break;

                case '.':
                    dot = true;
                    break;

                case '*':
                    if (dot == true)
                    {
                        dot = false;
                        precision = PRECISION_ARG;
                    }
                    else
                        width = WIDTH_ARG;
                    break;

                case 'h':
                    length = LENGTH_SHORT;
                    break;

                case 'l':
                    length = LENGTH_LONG;
                    break;

                case 'L':
                    length = LENGTH_DOUBLE;
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (dot == true)
                    {
                        dot = false;
                        precision = atoi(format);
                    }
                    else
                    {
                        width = atoi(format);
                    }
                    while (isdigit(*format))
                        format++;
                    format--;
                    break;

                default:
                    done = true;
                    format--;
                    break;
                }
                format++;
            }

            // Get specifier.
            spec = *format;
            format++;

            // Character.
            if (spec == 'c' || spec == '%')
            {
                if (length != LENGTH_WIDE)
                {
                    char *c = (char*) va_arg(arg, int*);
                    *c = fgetc(stream);
                    args_read++;
                }
                continue;
            }

            // String.
            if (spec == 's')
            {
                // Get precision from va_arg if specified.
                if (precision == PRECISION_ARG)
                    precision = (int) va_arg(arg, int);

                // Get pointer to string.
                char *s = (char*) va_arg(arg, char*);

                if (precision > 0)
                {
                    fgets(s, precision, stream);
                    format += precision;
                    args_read++;
                }
                else
                {
                    fgets(s, INT_MAX, stream);

                    // Strip newline character.
                    size_t tmp_len = strlen(s);
                    s[tmp_len - 1] = '\0';

                    args_read++;
                }
                continue;
            }

            // Signed decimal integer.
            if (spec == 'd' || spec == 'i')
            {
                // Get width from va_arg if specified.
                if (width == WIDTH_ARG)
                    width = (int) va_arg(arg, int);

                int tmp_max = 100 + width;
                int tmp_len = 0;
                char tmp[tmp_max];
                int pad;
                char c = fgetc(stream);
                while (isdigit(c))
                {
                    tmp[tmp_len] = c;
                    tmp_len++;
                    c = fgetc(stream);
                }
                tmp[tmp_len] = '\0';

                // Get integer.
                if (length == LENGTH_SHORT)
                {
                    short *n = (short*) va_arg(arg, int*);
                    *n = atol(tmp);
                }
                else if (length == LENGTH_LONG)
                {
                    long *n = (long*) va_arg(arg, long*);
                    *n = atol(tmp);
                }
                else
                {
                    int *n = (int*) va_arg(arg, int*);
                    *n = atoi(tmp);
                }
                args_read++;
                continue;
            }
            format++;

            // Reset tags.
            left_justify = false;
            force_sign = false;
            force_space = false;
            force_spec = false;
            zero_pad = false;
            width = -1;
            precision = -1;
            length = LENGTH_DEFAULT;
        }

        len++;
    }

    return (args_read);
}

int fscanf(FILE *stream, const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vfscanf(stream, format, arg);
    va_end(arg);
    return (ret);
}

int vscanf(const char *format, va_list arg)
{
    return ( vfscanf(stdin, format, arg) );
}

int scanf(const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vfscanf(stdin, format, arg);
    va_end(arg);
    return (ret);
}

int vsscanf(const char *s, const char *format, va_list arg)
{
    int ret;
    FILE tmp;
    FILE *stream = &tmp;
    tmp.buf = (char*) s;
    tmp.pos = 0;
    tmp.len = strlen(s);
    tmp.max_len = SIZE_MAX;
    tmp.buf_mode = _IOFBF;
    tmp.io_mode = _IOS;
    tmp.write = &write_null;

    ret = vfscanf(stream, format, arg);
    return (ret);
}

int sscanf(const char *s, const char *format, ...)
{
    int ret;
    va_list arg;
    va_start(arg, format);
    ret = vsscanf(s, format, arg);
    va_end(arg);
    return (ret);
}

ssize_t write_null(const void *s, size_t n)
{
    // Do nothing.
    return (n);
}

ssize_t read_null(void *s, size_t n)
{
    char *p = (char*)s;

    for (size_t i = 0; i < n; i++)
    {
        p[n] = 0;
    }

    return (n);
}
