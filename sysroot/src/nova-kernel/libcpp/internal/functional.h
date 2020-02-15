/**
 * @file functional.h
 * @author Seth McBee
 * @date 2019-9-19
 */

#pragma once

#include <globals.h>

namespace std
{

template <class T>
struct less
{
    using first_argument_type = T;
    using second_argument_type = T;
    using result_type = bool;

    bool operator()(const T& t1, const T& t2) const
    {
        return t1 < t2;
    }
};

}
