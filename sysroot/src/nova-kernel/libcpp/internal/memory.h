/**
 * @file memory.h
 * @author Seth McBee
 * @date 2019-3-21
 */

#pragma once

#include <stddef.h>

#include <utility>

namespace std
{

template <class T>
class allocator
{
public:

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template <class U>
    struct rebind
    {
        using other = allocator<U>;
    };

    T* allocate(size_t n)
    {
        return new T[n];
    }

    void deallocate(T* t, size_t n)
    {
        delete[] t;
    }

    template <class... Args>
    void construct(T* t, Args&&... args)
    {
        void* p = (void*)t;
        new (p) T(forward<Args>(args)...);
    }

    void destroy(T* t)
    {
        t->~T();
    }
};

template <class T>
class default_delete
{
public:

    void operator()(T* t) const
    {
        delete t;
    }
};

template <class T>
class default_delete<T[]>
{
public:

    void operator()(T* t) const
    {
        delete[] t;
    }
};

template <class T, class D = default_delete<T>>
class unique_ptr
{
public:

    using element_type = T;
    using deleter_type = D;
    using pointer = T*;

    constexpr unique_ptr() noexcept
    {
        data = nullptr;
    }

    constexpr explicit unique_ptr(nullptr_t) noexcept
    {
        data = nullptr;
    }

    explicit unique_ptr(T* t) noexcept
    {
        data = t;
    }

    unique_ptr(T* t, D d) noexcept
    {
        data = t;
        deleter = d;
    }

    ~unique_ptr()
    {
        deleter(data);
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
        deleter(data);
        data = nullptr;
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return (data != nullptr);
    }

    void reset(pointer t = nullptr) noexcept
    {
        deleter(data);
        data = t;
    }

    void release() noexcept
    {
        data = nullptr;
    }

    pointer get() noexcept
    {
        return data;
    }

    element_type& operator*() const
    {
        return *data;
    }

    pointer operator->() const
    {
        return data;
    }

private:

    pointer data;
    D deleter;
};

/// unique_ptr partial specialization for arrays.
template <class T, class D>
class unique_ptr<T[], D>
{
public:

	constexpr unique_ptr() noexcept
    {
        data = nullptr;
    }

    constexpr explicit unique_ptr(nullptr_t) noexcept
    {
        data = nullptr;
    }

    explicit unique_ptr(T* t, D d) noexcept
    {
        data = t;
        deleter = d;
    }

    ~unique_ptr()
    {
        deleter(data);
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
        deleter(data);
        data = nullptr;
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return (data != nullptr);
    }

    void reset(T* t = nullptr) noexcept
    {
        deleter(data);
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

    T& operator*() = delete;
    T* operator->() = delete;

    T& operator[](size_t n)
    {
        return data[n];
    }

private:

    T* data;;
    D deleter;
};

} // namespace std
