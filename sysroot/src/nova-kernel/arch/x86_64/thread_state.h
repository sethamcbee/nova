/**
 * @file process.h
 * @author Seth McBee
 * @date 2020-4-28
 * @brief x86_64 thread state.
 */

#pragma once

#include <globals.h>

struct Thread_State
{
    size_t rsp;
    size_t cr3;
    size_t rsp0; // Kernel stack.
};
