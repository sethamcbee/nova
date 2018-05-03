// Authors: Seth McBee
// Created: 2018-4-27
// Description: STDIO.

#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>

// Error codes.
#define EOF -1

// File buffer modes.
#define _IONBF 0    // No buffer.
#define _IOLBF 1    // Line-buffer.
#define _IOFBF 2    // Full-buffer.

// IO modes.
#define _IOI 0      // Input only.
#define _IOO 1      // Output only.
#define _IOS 2      // String virtual mode.

// C file abstraction.
typedef struct FILE
{
    volatile char* buf;
    volatile size_t pos;
    volatile size_t len;
    volatile size_t max_len;
    volatile int buf_mode;
    volatile int io_mode;
    volatile union
    {
        ssize_t (*write)(const void *s, size_t n);
        ssize_t (*read)(void *s, size_t n);
    };
} FILE;

FILE *stdin;
FILE *stdout;
FILE *stderr;

// Helper functions that operate directly on raw streams.
int rgetc(FILE *stream);
int rputc(int c, FILE *stream);
int rungetc(int c, FILE *stream);

// Initialize STDIO interface. Must be called before any related
// features may be used.
void stdio_init(void);

// Flush an output stream.
int fflush(FILE *stream);

// Clears the error flags for a stream.
void clearerr(FILE *stream);

// Write a given number of characters to a stream.
int fputn(const char *s, size_t n, FILE *stream);

// Writes a single character to a stream.
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);

// Write a single character to stdout.
int putchar(int c);

// Write a C-string to a stream.
int fputs(const char *s, FILE *stream);

// Write a C-string to stdout.
int puts(const char *s);

// Get a single character from a stream.
int fgetc(FILE *stream);
int getc(FILE *stream);

// Puts a character back into an input stream.
int ungetc(int c, FILE *stream);

// Get a single character from stdin.
int getchar(void);

// Get a string from stdin, not including the final newline.
char* gets(char *s);

// Get a a line from a stream, up to n-1 characters.
char* fgets(char *s, int n, FILE *stream);

// Write a formatted string to a stream.
int vfprintf(FILE *stream, const char *format, va_list arg);
int fprintf(FILE *stream, const char *format, ...);

// Write a formatted string to stdout.
int vprintf(const char *format, va_list arg);
int printf(const char *format, ...);

// Write a formatted string to a string.
int vsprintf(char *s, const char *format, va_list arg);
int sprintf(char *s, const char *format, ...);

// Read a formatted string from a stream.
int vfscanf(FILE *stream, const char *format, va_list arg);
int fscanf(FILE *stream, const char *format, ...);

// Read a formatted string from stdin.
int vscanf(const char *format, va_list arg);
int scanf(const char *format, ...);

// Read a formatted string from another string.
int vsscanf(const char *s, const char *format, va_list arg);
int sscanf(const char *s, const char *format, ...);

// Null write interface.
ssize_t write_null(const void *s, size_t n);

// Null read interface.
ssize_t read_null(void *s, size_t n);

#endif // STDIO_H
