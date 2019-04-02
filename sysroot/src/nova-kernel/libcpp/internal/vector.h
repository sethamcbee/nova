/**
 * @file vector.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <stdint.h>

#include <kernel.h>

const float VECTOR_GROWTH_RATE = 2.0f;

namespace std
{

template <class T, class Alloc = allocator<T>>
class vector
{
public:

    using value_type = T;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*; /// TODO: implement allocator_traits.
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    /** Constructors. **/
    
    vector() {};
    
    explicit vector(const Alloc& alloc)
        : vector_alloc(alloc) {}
        
    explicit vector(size_t n, const Alloc& alloc = Alloc())
        : vector_alloc(alloc), count(n), max_count(n)
    {
        data = vector_alloc.allocate(n);
    }
    
    vector(size_t n, const T& t, const Alloc& alloc = Alloc())
        : vector_alloc(alloc), count(n), max_count(n)
    {
        data = vector_alloc.allocate(n);
        for (size_t i = 0; i < n; ++i)
        {
            data[i] = t;
        }
    }
    
    vector(const vector& other)
    {
        count = other.count;
        max_count = other.max_count;
        vector_alloc = other.vector_alloc;
        data = vector_alloc.allocate(max_count);
        for (size_t i = 0; i < count; ++i)
        {
            data[i] = other.data[i];
        }
    }
    
    vector(const vector& other, const Alloc& alloc)
        : vector_alloc(alloc)
    {
        count = other.count;
        max_count = other.max_count;
        vector_alloc = alloc;
        data = vector_alloc.allocate(max_count);
        for (size_t i = 0; i < count; ++i)
        {
            data[i] = other.data[i];
        }
    }
    
    vector(vector&& other)
    {
        count = other.count;
        max_count = other.max_count;
        other.count = 0;
        other.max_count = 0;
        vector_alloc = std::move(other.vector_alloc);
        data = other.data;
        other.data = nullptr;
    }
    
    vector(vector&& other, const Alloc& alloc)
        : vector_alloc(alloc)
    {
        count = other.count;
        max_count = other.max_count;
        other.count = 0;
        other.max_count = 0;
        data = other.data;
        other.data = nullptr;
    }
    
    vector(initializer_list<value_type> il, const Alloc& alloc = Alloc())
        : vector_alloc(alloc)
    {
        for (const auto& n : il)
        {
            push_back(n);
        }
    }
    
    /** Destructor. **/
    
    ~vector()
    {
        vector_alloc.deallocate(data, max_count);
    }
    
    /** Assignment operators. **/
    
    vector& operator=(const vector& other)
    {
        vector_alloc.deallocate(data, max_count);
        count = other.count;
        max_count = other.max_count;
        vector_alloc = other.vector_alloc;
        data = vector_alloc.allocate(max_count);
        for (size_t i = 0; i < count; ++i)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
    
    vector& operator=(vector&& other)
    {
        vector_alloc.deallocate(data, max_count);
        count = other.count;
        max_count = other.max_count;
        other.count = 0;
        other.max_count = 0;
        vector_alloc = std::move(other.vector_alloc);
        data = vector_alloc.allocate(count);
        for (size_t i = 0; i < count; ++i)
        {
            data[i] = std::move(other.data[i]);
        }
        other.data = 0;
        return *this;
    }
    
    /** Status. **/
    
    bool empty() const
    {
        return count == 0;
    }
    
    size_t size() const
    {
        return count;
    }
    
    size_t max_size() const
    {
        return SIZE_MAX;
    }
    
    size_t capacity() const
    {
        return max_count;
    }
    
    /** Mutators. **/
    
    void resize(size_t n)
    {
        // This could be optimized to only reallocate if the size
        // is greater than the current capacity.
        T* new_data = vector_alloc.allocate(n);
        size_t upper = min(n, max_count);
        for (size_t i = 0; i < upper; ++i)
        {
            new_data[i] = data[i];
        }
        vector_alloc.deallocate(data, max_count);
        data = new_data;
        count = n;
        max_count = n;
    }
    
    void resize(size_t n, const T& t)
    {
        T* new_data = vector_alloc.allocate(n);
        size_t upper = min(n, max_count);
        for (size_t i = 0; i < n; ++i)
        {
            new_data[i] = data[i];
        }
        for (size_t i = upper; i < n; ++i)
        {
            new_data[i] = t;
        }
        vector_alloc.deallocate(data, max_count);
        data = new_data;
        count = n;
        max_count = n;
    }
    
    void reserve(size_t n)
    {
        if (n <= max_count)
        {
            return;
        }
        
        T* new_data = vector_alloc.allocate(n);
        for (size_t i = 0; i < count; ++i)
        {
            new_data[i] = data[i];
        }
        vector_alloc.deallocate(data, max_count);
        data = new_data;
        max_count = n;
    }
    
    T& front()
    {
        return data[0];
    }
    
    const T& front() const
    {
        return data[0];
    }
    
    T& back()
    {
        return data[count - 1];
    }
    
    const T& back() const
    {
        return data[count - 1];
    }
    
    T& operator[](size_t pos)
    {
        return data[pos];
    }
    
    const T& operator[](size_t pos) const
    {
        return data[pos];
    }
    
    void clear() noexcept
    {
        for (size_t i = 0; i < count; ++i)
        {
            data[i].~T();
        }
        count = 0;
    }
    
    void push_back(const T& t)
    {
        if (count == 0 || count == (max_count - 1))
        {
            reserve(max(2.0f, max_count * VECTOR_GROWTH_RATE));
        }
        
        data[count] = t;
        ++count;
    }
    
    void pop_back()
    {
        --count;
    }
    
    iterator begin() noexcept
    {
        return iterator(data);
    }
    
    const_iterator begin() const noexcept
    {
        return const_iterator(data);
    }
    
    iterator end() noexcept
    {
        return iterator(data + count);
    }
    
    const_iterator end() const noexcept
    {
        return const_iterator(data + count);
    }
    
private:

    T* data = nullptr;
    size_t count = 0;
    size_t max_count = 0;
    Alloc vector_alloc;
};

}

