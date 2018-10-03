/**
 * @file scheduler.c
 * @author Seth McBee
 * @date 2018-9-6
 * @brief Scheduler for process switching.
 */

#include <globals.h>

#include <proc/process.h>
#include <proc/scheduler.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/registers.h>
#endif // ARCH_X86_64

bool task_tick(void)
{
    // Disable scheduler.
    //return (false);

    // Verify that scheduler is running.
    if (cur_task == NULL)
    {
        return (false);
    }

    // Verify that this isn't the only task.
    if (cur_task->next == cur_task)
    {
        return (false);
    }

    cur_task->ticks--;

    if (cur_task->ticks == 0)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void task_next(void)
{
    Process* proc;

    // Set up this task for its next turn.
    cur_task->ticks = DEFAULT_TICKS;
    //cur_task->proc->rsp0 = tss.rsp0;

    // Change task.
    cur_task = cur_task->next;
    proc = cur_task->proc;
    cpu_proc(proc);
}

void task_add(Task* new_task)
{
    Task* tmp;
    tmp = cur_task->next;
    cur_task->next = new_task;
    new_task->next = tmp;
}

Registers* task_get_reg(void)
{
    Registers* ptr;
    ptr = &(cur_task->proc->reg);
    return (ptr);
}
