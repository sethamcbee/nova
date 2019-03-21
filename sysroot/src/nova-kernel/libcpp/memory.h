/**
 * @file memory.h
 * @author Seth McBee
 * @date 2019-3-21
 */

#pragma once

#include <stddef.h>

namespace std
{

template <class T>
class unique_ptr
{
public:
    
    constexpr unique_ptr() noexcept
    {
        data = nullptr;
    }

    constexpr unique_ptr(nullptr_t) noexcept
    {
        data = nullptr;
    }

    explicit unique_ptr(T* t) noexcept
    {
        data = t;
    }

    ~unique_ptr()
    {
        delete data;
    }

    unique_ptr(unique_ptr<T>& other) = delete;

    unique_ptr(unique_ptr<T>&& other) noexcept
    {
        data = other.data;
        other.data = nullptr;
    }
    
    unique_ptr<T>& operator=(const unique_ptr<T>& other) = delete;

    unique_ptr<T>& operator=(unique_ptr<T>&& other) noexcept
    {
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    unique_ptr<T>& operator=(nullptr_t t)
    {
        delete data;
        data = nullptr;
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return (data != nullptr);
    }

    void reset(T* t) noexcept
    {
        delete data;
        data = t;
    }

    void release() noexcept
    {
        data = nullptr;
    }

    T* get() noexcept
    {
        return data;
    }

    T& operator*() const
    {
        return *data;
    }

    T* operator->() const
    {
        return data;
    }

private:
    T* data = nullptr;
};

} // namespace std
