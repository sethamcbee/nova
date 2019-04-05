/**
 * @file algorithm.h
 * @author Seth McBee
 * @date 2019-3-28
 */

#pragma once

namespace std
{

template <class T>
const T& min(const T& a, const T& b)
{
    if (b < a)
    {
        return b;
    }
    else
    {
        return a;
    }
}


template <class T>
const T& max(const T& a, const T& b)
{
    if (a < b)
    {
        return b;
    }
    else
    {
        return a;
    }
}


template <class It1, class It2>
constexpr void iter_swap(It1 it1, It2 it2)
{
    using std::swap;
    swap(*it1, *it2);
}


template <class It1, class It2>
constexpr It2 swap_ranges(It1 src, It1 end, It2 dest)
{
    while (src != end)
    {
        iter_swap(src, dest);
        ++src;
        ++dest;
    }
    return dest;
}

}
