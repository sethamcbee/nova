// Authors: Seth McBee
// Created: 2018-9-6
// Description: Scheduler for process switching.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <proc/process.h>
#include <proc/scheduler.h>

#include <arch/x86_64/cpu.h>

void task_next(void)
{
    cur_task = cur_task->next;

    Process* proc = cur_task->proc;
    cpu_ring3(proc->ex, proc->stk);
}

void task_add(Task* new_task)
{
    Task* tmp;
    tmp = cur_task->next;
    cur_task->next = new_task;
    new_task->next = tmp;
}
