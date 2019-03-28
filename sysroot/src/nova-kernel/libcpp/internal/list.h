/**
 * @file list.h
 * @author Seth McBee
 * @date 2019-3-27
 */

#pragma once

#include <memory>
#include <utility>
#include <stdint.h>

namespace std
{

template <class T, class Allocator>
struct ListNode
{
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

template <class T, class Allocator = std::allocator<T>>
class list
{
public:

	/** Constructors. **/

    list() {}
    
    explicit list(const Allocator& alloc) : node_alloc(alloc) {}
    
    explicit list(size_t n, const T& val = T(), const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        for (size_t i = 0; i < n; ++i)
        {
            push_back(val);
        }
    }
    
    explicit list(size_t n, const Allocator& alloc = Allocator())
        : node_alloc(alloc)
    {
        for (size_t i = 0; i < n; ++i)
        {
            push_back(T());
        }
    }
    
    /** Destructors. **/
    ~list()
    {
		clear();
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
		return *(tail->data);
	}
	
	const T& back() const
	{
		return *(tail->data);
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
		tail = nullptr;
		node_count = 0;
	}
	
	void push_back(const T& t)
	{
		if (head == nullptr)
		{
			head = new ListNode<T, Allocator>(t, node_alloc);
			tail = head;
		}
		else
		{
			tail->next = new ListNode<T, Allocator>(t, node_alloc);
			tail->next->prev = tail;
			tail = tail->next;
		}
		++node_count;
	}
	
	void push_back(T&& t)
	{
		if (head == nullptr)
		{
			head = new ListNode<T, Allocator>(std::move(t), node_alloc);
			tail = head;
		}
		else
		{
			tail->next = new ListNode<T, Allocator>(std::move(t), node_alloc);
			tail->next->prev = tail;
			tail = tail->next;
		}
		++node_count;
	}
	
	void push_front(const T& t)
	{
		if (head == nullptr)
		{
			head = new ListNode<T, Allocator>(t, node_alloc);
			tail = head;
		}
		else
		{
			auto next = head;
			head = new ListNode<T, Allocator>(t, node_alloc);
			head->next = next;
			
			if (next != nullptr)
			{
				next->prev = head;
			}
		}
		++node_count;
	}
	
	void push_front(T&& t)
	{
		if (head == nullptr)
		{
			head = new ListNode<T, Allocator>(std::move(t), node_alloc);
			tail = head;
		}
		else
		{
			auto next = head;
			head = new ListNode<T, Allocator>(std::move(t), node_alloc);
			head->next = next;
			
			if (next != nullptr)
			{
				next->prev = head;
			}
		}
		++node_count;
	}
	
	void pop_back()
	{
		auto prev = tail->prev;
		delete tail;
		tail = prev;
		--node_count;
	}
	
	void pop_front()
	{
		auto next = head->next;
		delete head;
		head = next;
		--node_count;
	}
    
private:

    ListNode<T, Allocator>* head = nullptr;
    ListNode<T, Allocator>* tail = nullptr;
    Allocator node_alloc;
    size_t node_count = 0;
};

}
