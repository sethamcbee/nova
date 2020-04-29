/**
 * @file mutex.h
 * @authors Seth McBee
 * @date 2020-4-29
 */

#pragma once

#include <proc/thread.h>

namespace std
{

struct mutex_handle
{
    volatile int locked = 0;
    Thread* owner = nullptr;
};

class mutex
{
public:

    using native_handle_type = mutex_handle;

    constexpr mutex() noexcept {}

    mutex(const mutex& other) = delete;

    native_handle_type native_handle()
    {
        return handle;
    }

    void lock()
    {
        // Acquire lock.
        while (!__sync_bool_compare_and_swap(&handle.locked, 0, 1))
        {
            // TODO: Properly yield.
            asm volatile ("hlt \n");
        }
        handle.owner = current_thread;
    }

    bool try_lock()
    {
        if (__sync_bool_compare_and_swap(&handle.locked, 0, 1))
        {
            handle.owner = current_thread;
            return true;
        }
        else
        {
            return false;
        }
    }

    void unlock()
    {
        if (handle.owner == current_thread)
        {
            handle.owner = nullptr;
            handle.locked = 0;
        }
    }

private:

    native_handle_type handle;
};

template <class Mutex>
class lock_guard
{
public:

    using mutex_type = Mutex;

    explicit lock_guard(mutex_type& m) : m_(m)
    {
        m_.lock();
    }

    lock_guard(const lock_guard& other) = delete;

    ~lock_guard()
    {
        m_.unlock();
    }

private:

    mutex_type& m_;
};

}
