/**
 * @file stack.h
 * @author Seth McBee
 * @date 2019-9-24
 */

#pragma once

#include <globals.h>

#include <deque>

namespace std
{

template <class T, class Container = deque<T>>
class stack
{
public:

    using value_type = T;
    using container_type = Container;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;

    /** Constructors. **/

    explicit stack(const container_type& c) : data(c) {}

    explicit stack(container_type&& c = container_type())
        : data(c) {}

    /** Member functions. **/

    bool empty() const
    {
        return data.size() == 0;
    }

    size_type size() const
    {
        return data.size();
    }

    reference top()
    {
        return data.back();
    }

    const_reference top() const
    {
        return data.back();
    }

    void push(const_reference e)
    {
        data.push_back(e);
    }

    void pop()
    {
        data.pop_back();
    }

private:

    container_type data;
};

}
