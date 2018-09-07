// Authors: Seth McBee
// Created: 2018-9-6
// Description: Kernel and userpace process structs.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/registers.h>

typedef struct
{
    // Virtual address of the register storage.
    Registers reg;

    // RSP0 to store in TSS.
    uint64_t rsp0;

    // Kernel = 0, User = 3;
    uint8_t priv;
} Process;
