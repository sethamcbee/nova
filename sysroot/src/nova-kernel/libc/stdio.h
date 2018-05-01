// Authors: Seth McBee
// Created: 2018-4-27
// Description: STDIO.

#ifndef STDIO_H
#define STDIO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Error codes.
#define EOF -1

// File buffer modes.
#define _IONBF 0    // No buffer.
#define _IOLBF 1    // Line-buffer.
#define _IOFBF 2    // Full-buffer.

// IO modes.
#define _IOI 0      // Input only.
#define _IOO 1      // Output only.

// C file abstraction.
typedef struct FILE
{
    volatile char* buf;
    volatile size_t pos;
    volatile size_t len;
    volatile size_t max_len;
    volatile int buf_mode;
    volatile int io_mode;
} FILE;

FILE *stdin;
FILE *stdout;
FILE *stderr;

// Initializes STDIO interface. Must be called before any related
// features may be used.
void stdio_init(void);

// Flushes output stream.
int fflush(FILE *stream);

// Writes a single character to a stream.
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);

// Writes a single character to stdout.
int putchar(int c);

// Writes a C-string to a stream.
int fputs(const char *s, FILE *stream);

// Writes a C-string to stdout.
int puts(const char *s);

// Gets a single character from a stream.
int fgetc(FILE *stream);
int getc(FILE *stream);

// Gets a single character from stdin.
int getchar(void);

// Gets a string from stdin, not including the final newline.
char* gets(char *s);

// Gets a a line from a stream, up to n-1 characters.
char* fgets(char *s, int n, FILE *stream);

#endif // STDIO_H
