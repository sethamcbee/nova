/**
 * @file array.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

#include <initializer_list>
#include <iterator>

#include <kernel.h>

namespace std
{

template <class T, size_t N>
struct array
{
public:

    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

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
    
    /** Iterators. **/
    
    constexpr iterator begin()
    {
        return contents;
    }
    
    constexpr const_iterator begin() const
    {
        return contents;
    }
    
    constexpr iterator end()
    {
        return contents + N;
    }
    
    constexpr const_iterator end() const
    {
        return contents + N;
    }
    
    constexpr const_iterator cbegin() const
    {
        return contents;
    }
    
    constexpr const_iterator cend() const
    {
        return contents + N;
    }
    
    constexpr reverse_iterator rbegin()
    {
        auto it = contents + N - 1;
        return reverse_iterator(it);
    }
    
    constexpr const_reverse_iterator rbegin() const
    {
        auto it = contents + N - 1;
        return const_reverse_iterator(it);
    }
    
    constexpr reverse_iterator rend()
    {
        auto it = contents - 1;
        return reverse_iterator(it);
    }
    
    constexpr const_reverse_iterator rend() const
    {
        auto it = contents - 1;
        return const_reverse_iterator(it);
    } 

    T contents[N];
};

}
