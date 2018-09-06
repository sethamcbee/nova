// Authors: Seth McBee
// Created: 2018-9-6
// Description: Kernel and userpace process structs.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    // Virtual location of the last stack location.
    size_t stk;

    // Virtual location of the last instruction.
    size_t ex;
} Process;
