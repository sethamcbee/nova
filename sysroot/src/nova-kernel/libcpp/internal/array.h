/**
 * @file array.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

#include <kernel.h>

namespace std
{

template <class T, size_t N>
struct array
{
public:

    /** Accessors. **/

    constexpr T* data() noexcept
    {
        return contents;
    }
    
    constexpr T& front()
    {
        return contents[0];
    }
    
    constexpr const T& front() const
    {
        return contents[0];
    }
    
    constexpr T& back()
    {
        return contents[N - 1];
    }
    
    constexpr const T& back() const
    {
        return contents[N - 1];
    }
    
    constexpr T& operator[](size_t pos)
    {
        return contents[pos];
    }
    
    constexpr const T& operator[](size_t pos) const
    {
        return contents[pos];
    }
    
    constexpr T& at(size_t pos)
    {
        if (pos < N)
        {
            return contents[pos];
        }
        else
        {
            kernel_panic("Out-of-range array index.");
        }
    }
    
    /** Status. **/

    constexpr bool empty() const
    {
        return N == 0;
    }
    
    constexpr size_t size() const
    {
        return N;
    }
    
    constexpr size_t max_size() const
    {
        return N;
    }

    T contents[N];
};

}
