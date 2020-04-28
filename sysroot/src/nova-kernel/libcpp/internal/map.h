/**
 * @file map.h
 * @author Seth McBee
 * @date 2019-4-5
 */

#pragma once

#include <globals.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stack>
#include <utility>
#include <stdint.h>
#include <stdio.h>

namespace std
{

template <class Data>
struct MapNode
{
    MapNode() {}
    MapNode(const Data& d) : data(d) {}

    size_t height = 1;
    Data data;
    MapNode<Data>* left;
    MapNode<Data>* right;
};

template <class Data, class Compare>
struct MapTree
{
    MapTree() {}

    ~MapTree()
    {
        if (root == nullptr)
        {
            return;
        }

        stack<MapNode<Data>*> stk;
        stk.push(root);
        while (!stk.empty())
        {
            auto cur = stk.top();
            stk.pop();
            if (cur->left != nullptr)
            {
                stk.push(cur->left);
            }
            if (cur->right != nullptr)
            {
                stk.push(cur->right);
            }
            delete cur;
        }
    }

    int calc_height(const MapNode<Data>* root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        else
        {
            return root->height;
        }
    }

    int calc_balance(const MapNode<Data>* root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        else
        {
            int l = calc_height(root->left);
            int r = calc_height(root->right);
            return l - r;
        }
    }

    void update_height(MapNode<Data>* root)
    {
        int l = calc_height(root->left);
        int r = calc_height(root->right);
        root->height = 1 + max(l, r);
    }

    void rotate_left(MapNode<Data>*& root)
    {
        auto new_root = root->right;
        auto new_right = new_root->left;

        // Rotate.
        new_root->left = root;
        root->right = new_right;

        // Update heights.
        update_height(new_root->left->right);
        update_height(new_root);

        root = new_root;
    }

    void rotate_right(MapNode<Data>*& root)
    {
        auto new_root = root->left;
        auto new_left = new_root->left;

        // Rotate.
        new_root->right = root;
        root->left = new_left;

        update_height(new_root->right->left);
        update_height(new_root);

        root = new_root;
    }

    pair<MapNode<Data>*, bool> insert(const Data& d)
    {
        // Check for empty tree.
        if (root == nullptr)
        {
            root = new MapNode<Data>(d);
        }
        else
        {
            MapNode<Data>* cur = root;
            MapNode<Data>* prev = nullptr;
            stack<MapNode<Data>*> stk;

            // Build path to new node.
            while (cur != nullptr)
            {
                prev = cur;

                if (compare(d.first, cur->data.first))
                {
                    cur = cur->left;
                }
                else if (compare(cur->data.first, d.first))
                {
                    cur = cur->right;
                }
                else
                {
                    // Can't add duplicate node.
                    return {nullptr, false};
                }

                stk.push(prev);
            }

            // Add new node.
            if (compare(d.first, prev->data.first))
            {
                delete prev->left;
                prev->left = new MapNode<Data>(d);
            }
            else
            {
                delete prev->right;
                prev->right = new MapNode<Data>(d);
            }

            // Balance tree
            while (!stk.empty())
            {
                cur = stk.top();

                if (calc_balance(cur) > 1)
                {
                    // Check for L-R.
                    if (calc_balance(cur->left) < 0)
                    {
                        rotate_left(cur->left);
                    }

                    rotate_right(cur);
                }
                else if (calc_balance(cur) < -1)
                {
                    // Check for R-L.
                    if (calc_balance(cur->right) > 0)
                    {
                        rotate_right(cur->right);
                    }

                    rotate_left(cur);
                }

                update_height(cur);

                stk.pop();
            }
        }
    }

    void inorder(const MapNode<Data>* r)
    {
        if (r == nullptr)
        {
            return;
        }
        else
        {
            inorder(r->left);
            printf("[%d, %s], ", r->data.first, r->data.second);
            inorder(r->right);
        }
    }

    MapNode<Data>* root;
    Compare compare;
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

    /** Modifiers. **/

    void insert(const value_type& v)
    {
        tree.insert(v);
    }

    // TEST.
    void inorder()
    {
        tree.inorder(tree.root);
    }

private:

    MapTree<value_type, key_compare> tree;
    Allocator node_alloc;
    size_t node_count = 0;
};

}
