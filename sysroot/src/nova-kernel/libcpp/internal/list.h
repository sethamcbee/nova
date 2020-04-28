/**
 * @file list.h
 * @author Seth McBee
 * @date 2019-3-27
 */

#pragma once

#include <initializer_list>
#include <iterator>
#include <memory>
#include <utility>
#include <stdint.h>

namespace std
{

template <class T>
struct ListNode
{
    ListNode() {}
    ListNode(const T& t) : data(t) {}

    T data;
    ListNode<T>* prev = nullptr;
    ListNode<T>* next = nullptr;
};

template <class T>
struct ListIteratorBase
{
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

template <class T>
struct ListIterator : public ListIteratorBase<T>
{
    ListIterator(ListNode<T>* p) : pos(p) {}

    bool operator==(const ListIterator& other)
    {
        return pos == other.pos;
    }

    bool operator!=(const ListIterator& other)
    {
        return pos != other.pos;
    }

    T& operator*()
    {
        return pos->data;
    }

    T* operator->()
    {
        return pos->data;
    }

    ListIterator<T> operator+(ptrdiff_t n)
    {
        auto ret = pos;
        while (n > 0)
        {
            ret = ret->next;
            --n;
        }
        return ListIterator(ret);
    }

    ListIterator<T> operator-(ptrdiff_t n)
    {
        auto ret = pos;
        while (n > 0)
        {
            ret = ret->prev;
            --n;
        }
        return ListIterator(ret);
    }

    ListIterator<T> operator++()
    {
        pos = pos->next;
        return *this;
    }

    ListIterator<T> operator--()
    {
        pos = pos->prev;
        return *this;
    }

    ListIterator<T> operator+=(ptrdiff_t n)
    {
        while (n > 0)
        {
            pos = pos->next;
            --n;
        }
        return *this;
    }

    ListIterator<T> operator-=(ptrdiff_t n)
    {
        while (n > 0)
        {
            pos = pos->prev;
            --n;
        }
        return *this;
    }

    ListNode<T>* pos = nullptr;
};

template <class T>
struct ConstListIterator : public ListIteratorBase<T>
{
    ConstListIterator(const ListNode<T>* p) : pos(p) {}

    bool operator==(const ConstListIterator& other) const
    {
        return pos == other.pos;
    }

    bool operator!=(const ConstListIterator& other) const
    {
        return pos != other.pos;
    }

    const T& operator*() const
    {
        return pos->data;
    }

    const T* operator->() const
    {
        return pos->data;
    }

    ConstListIterator<T> operator+(ptrdiff_t n) const
    {
        auto ret = pos;
        while (n > 0)
        {
            ret = ret->next;
            --n;
        }
        return ConstListIterator(ret);
    }

    ConstListIterator<T> operator-(ptrdiff_t n) const
    {
        auto ret = pos;
        while (n > 0)
        {
            ret = ret->prev;
            --n;
        }
        return ConstListIterator(ret);
    }

    ConstListIterator<T> operator++()
    {
        pos = pos->next;
        return *this;
    }

    ConstListIterator<T> operator--()
    {
        pos = pos->prev;
        return *this;
    }

    ConstListIterator<T> operator+=(ptrdiff_t n)
    {
        while (n > 0)
        {
            pos = pos->next;
            --n;
        }
        return *this;
    }

    ConstListIterator<T> operator-=(ptrdiff_t n)
    {
        while (n > 0)
        {
            pos = pos->prev;
            --n;
        }
        return *this;
    }

    const ListNode<T>* pos = nullptr;
};

template <class T, class Alloc = allocator<T>>
class list
{
public:

    using value_type = T;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = ListIterator<T>;
    using const_iterator = ListIterator<T>;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    using NodeAlloc = typename Alloc::template rebind<ListNode<T>>::other;

    /** Constructors. **/

    explicit list(const allocator_type& a = allocator_type()) {}

    /** Destructor. **/

    ~list()
    {
        auto it = head;
        while (it != nullptr)
        {
            auto it_next = it->next;
            alloc.destroy(&*it);
            alloc.deallocate(&*it, 1);
            it = it_next;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    /** Iterators. **/

    iterator begin() noexcept
    {
        return iterator(head);
    }

    iterator end() noexcept
    {
        return iterator((ListNode<T>*)nullptr);
    }

    /** Member functions. **/

    bool empty() const
    {
        return count == 0;
    }

    size_type size() const
    {
        return count;
    }

    size_type max_size() const
    {
        return SIZE_MAX;
    }

    reference front()
    {
        return head->data;
    }

    const_reference front() const
    {
        return head->data;
    }

    reference back()
    {
        return tail->data;
    }

    const_reference back() const
    {
        return tail->data;
    }

    void push_front(const value_type& v)
    {
        auto new_head = alloc.allocate(1);
        alloc.construct(new_head, v);

        if (head == nullptr)
        {
            head = new_head;
            tail = new_head;
        }
        else
        {
            new_head->next = head;
            head->prev = new_head;
            head = new_head;
        }

        ++count;
    }

    void push_back(const value_type& v)
    {
        auto new_tail = alloc.allocate(1);
        alloc.construct(new_tail, v);

        if (head == nullptr)
        {
            head = new_tail;
            tail = new_tail;
        }
        else
        {
            new_tail->prev = tail;
            tail->next = new_tail;
            tail = new_tail;
        }

        ++count;
    }

    void pop_back()
    {
        if (tail == nullptr)
        {
            return;
        }

        if (tail == head)
        {
            alloc.destroy(&*tail);
            alloc.deallocate(&*tail, 1);
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            auto new_tail = tail->prev;
            alloc.destroy(&*tail);
            alloc.deallocate(&*tail, 1);
            new_tail->next = nullptr;
            tail = new_tail;
        }

        --count;
    }

    void pop_front()
    {
        if (head == nullptr)
        {
            return;
        }

        if (head == tail)
        {
            alloc.destroy(&*head);
            alloc.deallocate(&*head, 1);
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            auto new_head = head->next;
            alloc.destroy(&*head);
            alloc.deallocate(&*head, 1);
            new_head->prev = nullptr;
            head = new_head;
        }

        --count;
    }

private:

    ListNode<T>* head = nullptr;
    ListNode<T>* tail = nullptr;
    size_type count = 0;
    NodeAlloc alloc;
};

}
