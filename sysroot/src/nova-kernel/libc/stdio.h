/**
 * @file stdio.h
 * @author Seth McBee
 * @date 2018-4-27
 * @brief C standard library stdio header.
 */

#ifndef STDIO_H
#define STDIO_H

#include <globals.h>

/* Stream error codes. */

/// EOF error code.
#define EOF -1

/* Buffer modes. */

/// Un-buffered mode.
#define _IONBF 0

/// Line-buffered mode.
#define _IOLBF 1

/// Fully-buffered mode.
#define _IOFBF 2

/* Stream operation modes. */

/// Input-only stream.
#define _IOI 0

/// Output-only stream.
#define _IOO 1

/// String-stream.
#define _IOS 2

/// Stream/file abstraction.
typedef struct FILE
{
    /// Buffer for stream data.
    volatile char* buf;

    /// Current position in the stream.
    volatile size_t pos;

    /// Current length of the stream data.
    volatile size_t len;

    /// Maximum length of the stream data.
    volatile size_t max_len;

    /// Buffering mode.
    volatile int buf_mode;

    /// IO mode.
    volatile int io_mode;

    /// Stream operation function pointers.
    volatile union
    {
        ssize_t (*write)(const void* s, size_t n);
        ssize_t (*read)(void* s, size_t n);
    };
} FILE;

/// Standard input stream.
FILE* stdin;

/// Standard output stream.
FILE* stdout;

/// Standard error stream.
FILE* stderr;

/**
 * @brief Get byte from stream buffer. For internal use.
 *
 * @param stream Source stream.
 *
 * @return Byte from stream, or error code.
 */
int rgetc(FILE* stream);

/**
 * @brief Put byte in stream buffer. For internal use.
 *
 * @param c Byte to place in buffer.
 * @param stream Destination stream.
 *
 * @return Byte that was written to the buffer, or error code.
 */
int rputc(int c, FILE* stream);

/**
 * @brief Un-get byte from stream buffer. For internal use.
 *
 * @param c Byte to replace in buffer.
 * @param stream Destination stream.
 *
 * @return Byte that was placed back into the buffer, or error code.
 */
int rungetc(int c, FILE* stream);

/**
 * @brief Initialize STDIO interface. Must be called before any
 * related features may be used.
 */
void stdio_init(void);

/**
 * @brief Flush output buffer.
 *
 * @param stream Stream to be flushed.
 *
 * @return 0 if successful. Otherwise, return error code.
 */
int fflush(FILE *stream);

/**
 * @brief Clear error status on stream.
 *
 * @param stream Stream to be cleared.
 */
void clearerr(FILE *stream);

/**
 * @brief Write a given number of characters to a stream.
 *
 * @param s Data to be written.
 * @param n Number of bytes to write.
 * @param stream Destination stream.
 *
 * @return Number of bytes successfully written, or error code.
 */
int fputn(const char *s, size_t n, FILE *stream);

/**
 * @brief Write a single character to a stream.
 *
 * @param c Character to be written.
 * @param stream Destination stream.
 *
 * @return The character written, or EOF if an error occurred.
 * If an error occurred, ferror will be set accordingly.
 */
int fputc(int c, FILE *stream);


/// Write a single character to a stream. Identical to fputc().
int putc(int c, FILE *stream);

/**
 * @brief Write a single character to stdout.
 *
 * @see fputc()
 */
int putchar(int c);

/**
 * @brief Write a null-terminated string to a stream.
 *
 * @param s String to be written.
 * @param stream Destination stream.
 *
 * @return Number of bytes successfully written, or an error code.
 *
 * @see fputn()
 */
int fputs(const char *s, FILE *stream);

/**
 * @brief Write a null-terminated string to stdout.
 *
 * @param s String to be written.
 *
 * @return Number of bytes successfully written, or an error code.
 *
 * @see fputs()
 * @see fputn()
 */
int puts(const char *s);

/**
 * @brief Get a single character from a stream.
 *
 * @param stream Source stream.
 *
 * @return Character retrieved, or an error code.
 */
int fgetc(FILE *stream);

/**
 * @brief Get a single character from a stream.
 *
 * @see fgetc()
 */
int getc(FILE *stream);

/**
 * @brief Un-get a single character from a stream.
 *
 * @param c Character to place back.
 * @param stream Destination stream.
 *
 * @return Character put back, or EOF if an error occurred.
 */
int ungetc(int c, FILE *stream);

/**
 * @brief Get a single character from stdin.
 *
 * @see fgetc()
 */
int getchar(void);

/**
 * @brief Get a string from stdin, not including the trailing newline.
 *
 * @param s Destination in which to place the data.
 *
 * @return Pointer to destination, or EOF if an error occurred while
 * copying data.
 */
char* gets(char *s);

/// Get a line from a stream, up to n-1 characters.
char* fgets(char *s, int n, FILE *stream);

/// Write a formatted string to a stream with an argument list.
int vfprintf(FILE *stream, const char *format, va_list arg);

/// Write a formatted string to a stream with variable arguments.
int fprintf(FILE *stream, const char *format, ...);

/// Write a formatted string to stdout with an argument list.
int vprintf(const char *format, va_list arg);

/// Write a formatted string to stdout with variable arguments.
int printf(const char *format, ...);

/// Write a formatted string to another string with an argument list.
int vsprintf(char *s, const char *format, va_list arg);

/// Write a formatted string to another string with variable argument.
int sprintf(char *s, const char *format, ...);

/// Read a formatted string from a stream with an argument list.
int vfscanf(FILE *stream, const char *format, va_list arg);

/// Read a formatted string from a stream with variable arguments.
int fscanf(FILE *stream, const char *format, ...);

/// Read a formatted string from stdin with an argument list.
int vscanf(const char *format, va_list arg);

/// Read a formatted string from stdin with variable arguments.
int scanf(const char *format, ...);

/// Read a formatted string from another string with an argument list.
int vsscanf(const char *s, const char *format, va_list arg);

/// Read a formatted string from another string with variable arguments.
int sscanf(const char *s, const char *format, ...);

/// Null write interface.
ssize_t write_null(const void *s, size_t n);

/// Null read interface.
ssize_t read_null(void *s, size_t n);

#endif // STDIO_H
