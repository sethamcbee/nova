/**
 * @file map.h
 * @author Seth McBee
 * @date 2019-4-5
 */

#pragma once

#include <globals.h>

#include <functional>
#include <iterator>
#include <utility>

namespace std
{

template <class Key, class T, class Compare, class Allocator>
struct MapNode
{
    MapNode(Allocator& a) : alloc(a) {}

    MapNode(const T& k, const T& t, Allocator& a) : alloc(a)
    {
        data = make_pair(k, t);
    }

    ~MapNode()
    {
    }

    pair<Key, T> data;
    MapNode<Key, T, Compare, Allocator>* left = nullptr;
    MapNode<Key, T, Compare, Allocator>* right = nullptr;
    Allocator& alloc;
};

template <class Key, class T, class Compare, class Allocator>
class map;

template <class Key, class T, class Compare, class Allocator>
struct MapIteratorBase
{
    using iterator_category = bidirectional_iterator_tag;
    using value_type = pair<Key, T>;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

template <class Key, class T, class Compare, class Allocator>
struct MapIterator : public MapIteratorBase<Key, T, Compare, Allocator>
{
};

template <class Key, class T, class Compare, class Allocator>
struct ConstMapIterator : public MapIteratorBase<Key, T, Compare, Allocator>
{
};

template <class Key, class T, class Compare = less<Key>, class Allocator = allocator<pair<const Key, T>>>
class map
{
public:

    using key_type = Key;
    using mapped_type = T;
    using value_type = pair<const Key, T>;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = MapIterator<Key, T, Compare, Allocator>;
    using const_iterator = ConstMapIterator<Key, T, Compare, Allocator>;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
    using difference_type = typename iterator_traits<iterator>::difference_type;
    using size_type = size_t;

    /** Constructors. **/

    explicit map(
        const key_compare& c = key_compare(),
        const allocator_type& alloc = allocator_type())
        : comp(c), node_alloc(alloc) {}

    explicit map(const allocator_type& alloc)
        : comp(key_compare()), node_alloc(alloc) {}

private:

    MapNode<Key, T, Compare, Allocator>* root = nullptr;
    key_compare comp;
    Allocator node_alloc;
    size_t node_count = 0;
};

}
