/**
 * @file initializer_list.h
 * @author Seth McBee
 * @date 2019-4-1
 */

#pragma once

namespace std
{

template <class T>
class initializer_list
{
public:
    
    using value_type = T;
    using reference = const value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using iterator = const value_type*;
    using const_iterator = const value_type*;
    
    initializer_list() noexcept {}
    
    size_type size() noexcept
    {
        return count;
    }
    
    constexpr const_iterator begin() const noexcept
    {
        return const_iterator(data);
    }
    
    constexpr const_iterator end() const noexcept
    {
        return const_iterator(data + count);
    }
    
private:

    T* data = nullptr;
    size_type count = 0;
};

}
