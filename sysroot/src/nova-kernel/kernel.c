// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>

void kernel_main(void)
{
}

void kernel_panic(char* str, size_t len)
{
    // Ensures that the panic message is null-terminated.
    str[len] = '\0';

    // TODO: Print panic message and then halt.
}