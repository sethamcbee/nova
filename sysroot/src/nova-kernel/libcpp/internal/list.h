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

template <class T, class Allocator>
struct ListNode
{
    ListNode(Allocator& alloc) : node_alloc(alloc) {}
    
    ListNode(const T& t, Allocator& alloc)
        : node_alloc(alloc)
    {
        data = alloc.allocate(1);
        *data = t;
    }
    
    ~ListNode()
    {
        node_alloc.deallocate(data, 1);
    }
    
    T* data;
    ListNode<T, Allocator>* next = nullptr;
    ListNode<T, Allocator>* prev = nullptr;
    Allocator& node_alloc;
};

template <class T, class Allocator>
class list;

/// Iterator for list.
template <class T, class Allocator = allocator<T>>
class ListIteratorBase
{
public:

    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
    bool operator==(const ListIteratorBase<T, Allocator>& other) const
    {
        return pos == other.pos;
    }
    
    bool operator!=(const ListIteratorBase<T, Allocator>& other) const
    {
        return pos != other.pos;
    }
    
protected:

    friend class list<T, Allocator>;

    ListNode<T, Allocator>* pos = nullptr;
    list<T, Allocator>* container;
};

template <class T, class Allocator = allocator<T>>
class ConstListIterator : public ListIteratorBase<T, Allocator>
{
public:

    ConstListIterator(ListNode<T, Allocator>* node)
    {
        this->pos = node;
    }
    
    ConstListIterator<T, Allocator>& operator++()
    {
        this->pos = this->pos->next;
        return *this;
    }
    
    ConstListIterator<T, Allocator>& operator--()
    {
        this->pos = this->pos->prev;
        return *this;
    }

    const T& operator*() const
    {
        return *(this->pos->data);
    }
};

template <class T, class Allocator = allocator<T>>
class ListIterator : public ListIteratorBase<T, Allocator>
{
public:

    ListIterator(ListNode<T, Allocator>* node)
    {
        this->pos = node;
    }
    
    ListIterator<T, Allocator>& operator++()
    {
        this->pos = this->pos->next;
        return *this;
    }
    
    ListIterator<T, Allocator>& operator--()
    {
        this->pos = this->pos->prev;
        return *this;
    }

    T& operator*()
    {
        return *(this->pos->data);
    }
    
    operator ConstListIterator<T, Allocator>()
    {
        return *this;
    }
};

template <class T, class Allocator = allocator<T>>
class list
{
public:

    using value_type = T;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*; /// TODO: implement allocator_traits.
    using const_pointer = const T*;
    using iterator = ListIterator<value_type>;
    using const_iterator = ConstListIterator<value_type>;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

    /** Constructors. **/

    list()
    {
        end_node = new ListNode<T, Allocator>(node_alloc);
    }
    
    explicit list(const Allocator& alloc) : node_alloc(alloc) {}
    
    explicit list(size_t n, const T& val = T(), const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        end_node = new ListNode<T, Allocator>(node_alloc);
        for (size_t i = 0; i < n; ++i)
        {
            push_back(val);
        }
    }
    
    explicit list(size_t n, const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        end_node = new ListNode<T, Allocator>(node_alloc);
        for (size_t i = 0; i < n; ++i)
        {
            push_back(T());
        }
    }
    
    list(const list<T, Allocator>& other, const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        end_node = new ListNode<T, Allocator>(node_alloc);
        for (const auto& n : other)
        {
            push_back(n);
        }
    }
    
    list(initializer_list<value_type> il, const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        end_node = new ListNode<T, Allocator>(node_alloc);
        for (const auto& n : il)
        {
            push_back(n);
        }
    }
    
    /** Destructor. **/
    
    ~list()
    {
        delete end_node;
        clear();
    }
    
    /** Operators. **/
    
    list& operator=(const list<T, Allocator>& other)
    {
        clear();
        for (const auto& n : other)
        {
            push_back(n);
        }
        return *this;
    }
    
    /** Status. **/
    
    bool empty() const
    {
        return node_count == 0;
    }
    
    size_t size() const
    {
        return node_count;
    }
    
    size_t max_size() const
    {
        return SIZE_MAX;
    }
    
    /** Accessors. **/
    
    T& front()
    {
        return *(head->data);
    }
    
    const T& front() const
    {
        return *(head->data);
    }
    
    T& back()
    {
        return *(end_node->prev->data);
    }
    
    const T& back() const
    {
        return *(end_node->prev->data);
    }
    
    /** Mutators. **/
    
    void clear() noexcept
    {
        while (head != nullptr)
        {
            auto next = head->next;
            delete head;
            head = next;
        }
        node_count = 0;
    }
    
    void push_back(const T& t)
    {
        if (head == nullptr)
        {
            head = new ListNode<T, Allocator>(t, node_alloc);
            head->next = end_node;
            end_node->prev = head;
        }
        else
        {
            auto tail = end_node->prev;
            tail->next = new ListNode<T, Allocator>(t, node_alloc);
            tail->next->prev = tail;
            tail = tail->next;
            tail->next = end_node;
            end_node->prev = tail;
        }
        ++node_count;
    }
    
    void push_back(T&& t)
    {
        if (head == nullptr)
        {
            head = new ListNode<T, Allocator>(std::move(t), node_alloc);
            head->next = end_node;
            end_node->prev = head;
        }
        else
        {
            
            auto tail = end_node->prev;
            tail->next = new ListNode<T, Allocator>(std::move(t), node_alloc);
            tail->next->prev = tail;
            tail = tail->next;
            tail->next = end_node;
            end_node->prev = tail;
        }
        ++node_count;
    }
    
    void push_front(const T& t)
    {
        if (head == nullptr)
        {
            head = new ListNode<T, Allocator>(t, node_alloc);
            head->next = end_node;
            end_node->prev = head;
        }
        else
        {
            auto next = head;
            head = new ListNode<T, Allocator>(t, node_alloc);
            head->next = next;
            next->prev = head;
        }
        ++node_count;
    }
    
    void push_front(T&& t)
    {
        if (head == nullptr)
        {
            head = new ListNode<T, Allocator>(std::move(t), node_alloc);
            head->next = end_node;
            end_node->prev = head;
        }
        else
        {
            auto next = head;
            head = new ListNode<T, Allocator>(std::move(t), node_alloc);
            head->next = next;
            next->prev = head;
        }
        ++node_count;
    }
    
    void pop_back()
    {
        auto tail = end_node->prev;
        if (tail == nullptr)
        {
            return;
        }
        else
        {
            auto prev = tail->prev;
            delete tail;
            --node_count;
            end_node->prev = prev;
            if (prev == nullptr)
            {
                head = nullptr;
                return;
            }
            else
            {
                prev->next = end_node;
            }
        }
    }
    
    void pop_front()
    {
        auto next = head->next;
        if (next == end_node)
        {
            delete head;
            head = nullptr;
            end_node->prev = nullptr;
        }
        else
        {
            delete head;
            head = next;
            head->prev = nullptr;
        }
        --node_count;
    }
    
    /** Iterators. **/
    
    iterator begin() noexcept
    {
        return iterator(head);
    }
    
    const_iterator begin() const noexcept
    {
        return const_iterator(head);
    }
    
    iterator end() noexcept
    {
        return iterator(end_node);
    }
    
    const_iterator end() const noexcept
    {
        return const_iterator(end_node);
    }
    
    const_iterator cbegin() const noexcept
    {
        return const_iterator(head);
    }
    
    const_iterator cend() const noexcept
    {
        return const_iterator(end_node);
    }
    
    reverse_iterator rbegin() noexcept
    {
        iterator it(end_node->prev);
        return reverse_iterator(it);
    }
    
    const_reverse_iterator rbegin() const noexcept
    {
        const_iterator it(end_node->prev);
        return const_reverse_iterator(it);
    }
    
    reverse_iterator rend() noexcept
    {
        iterator it(nullptr);
        return reverse_iterator(it);
    }
    
    const_reverse_iterator rend() const noexcept
    {
        const_iterator it(nullptr);
        return const_reverse_iterator(it);
    }
    
    const_reverse_iterator crbegin() const noexcept
    {
        const_iterator it(end_node);
        return const_reverse_iterator(it);
    }
    
    const_reverse_iterator crend() const noexcept
    {
        const_iterator it(nullptr);
        return const_reverse_iterator(it);
    }

    ListNode<T, Allocator>* head = nullptr;
    ListNode<T, Allocator>* end_node;
    Allocator node_alloc;
    size_t node_count = 0;
};

}
