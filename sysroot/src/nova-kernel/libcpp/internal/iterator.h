/**
 * @file iterator.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

#include <type_traits>

namespace std
{

/// Iterator base class.
template <class C, class T, class D = ptrdiff_t, class P = T*, class R = T&>
struct iterator
{
    using iterator_category = C;
    using value_type = T;
    using difference_type = D;
    using pointer = P;
    using reference = R;
};

// Iterator categories.
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class It>
struct iterator_traits
{
    using iterator_category = typename It::iterator_category;
    using value_type = typename It::value_type;
    using difference_type = typename It::difference_type;
    using pointer = typename It::pointer;
    using reference = typename It::reference;
};

template <class T>
struct iterator_traits<T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
};

template <class T>
struct iterator_traits<const T*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
};


template <class It>
class reverse_iterator
{
public:

    using iterator_type = It;
    using iterator_category = typename iterator_traits<It>::iterator_category;
    using value_type = typename iterator_traits<It>::value_type;
    using difference_type = typename iterator_traits<It>::difference_type;
    using pointer = typename iterator_traits<It>::pointer;
    using reference = typename iterator_traits<It>::reference;

    reverse_iterator() {}
    
    explicit reverse_iterator(iterator_type other) : it(other) {}
    
    reverse_iterator(const reverse_iterator<It>& other) : it(other.it) {}
    
    iterator_type base() const
    {
        return it;
    }
    
    reference operator*()
    {
        return *it;
    }
    
    pointer operator->()
    {
        return &(*it);
    }
    
    reverse_iterator<It> operator+(difference_type n) const
    {
        return it - n;
    }
    
    reverse_iterator<It>& operator++()
    {
        --it;
        return *this;
    }
    
    reverse_iterator<It>& operator+=(difference_type n)
    {
        it -= n;
        return *this;
    }
    
    reverse_iterator<It> operator-(difference_type n) const
    {
        return it + n;
    }
    
    reverse_iterator<It>& operator--()
    {
        ++it;
        return *this;
    }
    
    reverse_iterator<It>& operator-=(difference_type n)
    {
        it +- n;
        return *this;
    }
    
    bool operator==(const reverse_iterator<It>& other)
    {
        return it == other.it;
    }
    
    bool operator!=(const reverse_iterator<It>& other)
    {
        return it != other.it;
    }

private:

    It it;
};


/** Iterator operations. **/


template <class C>
constexpr auto begin(C& c) -> decltype(c.begin());

template <class C>
constexpr auto begin(const C& c) -> decltype(c.begin());

template <class T, size_t N>
constexpr T* begin(T (&array)[N]) noexcept
{
    return &array;
}


template <class C>
constexpr auto end(C& c) -> decltype(c.end());

template <class C>
constexpr auto end(const C& c) -> decltype(c.end());

template <class T, size_t N>
constexpr T* end(T (&array)[N]) noexcept
{
    return &array[N - 1];
}


template <class It, class Distance, class Enable = void>
void advance_impl(It& it, Distance d)
{
    // Just iterate d times.
    while (d > 0)
    {
        ++it;
        --d;
    }
}

template <class It, class Distance,
    typename enable_if
    <
        is_same
        <
            random_access_iterator_tag,
            typename iterator_traits<It>::iterator_category
        >::value
    >::type
>
void advance_impl(It& it, Distance d)
{
    it += d;
}

template <class It, class Distance>
void advance(It& it, Distance d)
{
    advance_impl(it, d);
}


template <class It, class Enable = void>
auto distance_impl(It first, It last)
{
    // Just iterate until we find last.
    size_t count = 0;
    while (first != last)
    {
        ++count;
        ++first;
    }
    return count;
}

template <class It,
    typename enable_if
    <
        is_same
        <
            random_access_iterator_tag,
            typename iterator_traits<It>::iterator_category
        >::value
    >::type
>
auto distance_impl(It first, It last)
{
    return (last - first);
}

template <class It>
typename iterator_traits<It>::difference_type distance(It first, It last)
{
    return distance_impl(first, last);
}


template <class It, class Enable = void>
auto next_impl(It it, auto n = 1)
{
    while (n > 0)
    {
        --n;
        ++it;
    }
    return it;
}

template <class It,
    enable_if_t
    <
        is_same_v
        <
            random_access_iterator_tag,
            typename iterator_traits<It>::iterator_category
        >
    >
>
auto next_impl(It it, auto n = 1)
{
    return it + n;
}

template <class It>
It next(It it, typename iterator_traits<It>::difference_type n = 1)
{
    return next_impl(it, n);
}


template <class It, class Enable = void>
auto prev_impl(It it, auto n = 1)
{
    while (n > 0)
    {
        --n;
        --it;
    }
    return it;
}

template <class It,
    enable_if_t
    <
        is_same_v
        <
            random_access_iterator_tag,
            typename iterator_traits<It>::iterator_category
        >
    >
>
auto prev_impl(It it, auto n = 1)
{
    return it - n;
}

template <class It>
It prev(It it, typename iterator_traits<It>::difference_type n = 1)
{
    return prev_impl(it, n);
}

}
