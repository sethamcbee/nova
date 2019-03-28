/**
 * @file deque.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

#include <list>
#include <memory>

namespace std
{

/// Currently just a wrapper around list, so not particularly fast.
template <class T, class Alloc = allocator<T>>
class deque
{
public:

    /** Constructors. **/

    deque() {}

    explicit deque (const Alloc& alloc) : deque_alloc(alloc), data(deque_alloc) {}

    /** Destructors. **/

    ~deque() {}

    /** Status. **/

    bool empty() const
    {
        return data.empty();
    }

    size_t size() const
    {
        return data.size();
    }

    size_t max_size() const
    {
        return SIZE_MAX;
    }

    /** Accessors. **/

    T& front()
    {
        return data.front();
    }
    
    const T& front() const
    {
        return data.front();
    }
    
    T& back()
    {
        return data.back();
    }
    
    const T& back() const
    {
        return data.back();
    }
    
    deque<T>& operator[](size_t n)
    {
        auto it = data.head;
        for (size_t i = 0; i < n; ++i)
        {
            it = it->next;
        }
        return *it;
    }
    
    const deque<T>& operator [](size_t n) const
    {
        auto it = data.head;
        for (size_t i = 0; i < n; ++i)
        {
            it = it->next;
        }
        return *it;
    }

    /** Mutators. **/

    void push_back(const T& t)
    {
        data.push_back(t);
    }
    
    void push_front(const T& t)
    {
        data.push_front(t);
    }
    
    void pop_back()
    {
        data.pop_back();
    }
    
    void pop_front()
    {
        data.pop_front();
    }
    
    void clear()
    {
        data.clear();
    }

private:

    std::list<T, Alloc> data;
    Alloc deque_alloc;
};

}
