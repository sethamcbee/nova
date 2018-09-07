// Authors: Seth McBee
// Created: 2018-9-6
// Description: Scheduler for process switching.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <proc/process.h>

#define DEFAULT_TICKS 1

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

// Decrements task and returns whether the task should change.
bool task_tick(void);

// Switches context to the next task.
void task_next(void);

// Adds a task after the current one.
void task_add(Task* new_task);

// Gets pointer to the register storage of the current process.
Registers* task_get_reg(void);
