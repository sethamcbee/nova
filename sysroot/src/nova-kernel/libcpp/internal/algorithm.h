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

}
