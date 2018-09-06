// Authors: Seth McBee
// Created: 2018-9-6
// Description: Scheduler for process switching.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <proc/process.h>

// Stub. "next" just points to the next task, or itself if it is
// the last remaining process.
typedef struct Task Task;
struct Task
{
    Process* proc;

    // Number of clock ticks remaining until this task loses context.
    size_t ticks;

    Task* next;
};

// Points to the current task.
Task* cur_task;

// Switches context to the next task.
void task_next(void);

// Adds a task after the current one.
void task_add(Task* new_task);
