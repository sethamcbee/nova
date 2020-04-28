/**
 * @file process.h
 * @author Seth McBee
 * @date 2018-9-6
 * @brief Kernel and userspace processes.
 */

#pragma once

#include <globals.h>

#include <vector>

#include <proc/thread.h>

struct Process
{
    std::vector<Thread> threads;

    size_t id;

    char* name;
};
