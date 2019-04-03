/**
 * @file scheduler.h
 * @author Seth McBee
 * @date 2018-9-6
 * @brief Scheduler for process switching.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <globals.h>

#include <proc/process.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of ticks before a task switch.
 */
#define DEFAULT_TICKS 1

typedef struct Task Task;

/**
 * @brief Stores information pertaining to a task.
 * @note This should be replaced with a proper implementation.
 */
typedef struct Task
{
    /**
     * @brief Info for the process belonging to this task.
     */
    Process* proc;

    /**
     * @brief Number of ticks remaining before a forced task switch.
     */
    size_t ticks;

    /**
     * @brief Points to the next task to switch to.
     */
    Task* next;
} Task;

/**
 * @brief Points to the current task.
 */
extern Task* cur_task;

/**
 * @brief Decrement the tick count for the current task and return
 * whether a task switch should occur.
 *
 * @return Whether a task switch should occur.
 */
bool task_tick(void);

/**
 * @brief Switch context to the next task.
 */
void task_next(void);

/**
 * @brief Add a task to the list, immediately after the current
 * task.
 *
 * @param new_task New task to be added to the list.
 */
void task_add(Task* new_task);

/**
 * @brief Get a pointer to the current task's stored CPU state.
 */
Registers* task_get_reg(void);

#ifdef __cplusplus
}
#endif

#endif // SCHEDULER_H
