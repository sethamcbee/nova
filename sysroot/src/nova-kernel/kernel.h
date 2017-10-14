// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel functions.

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Main kernel. Should only be called once.
void kernel_main(void);

// Kernel panic. Prints panic message and then halts infinitely.
void kernel_panic(char *str, size_t len);

#endif // _KERNEL_H_