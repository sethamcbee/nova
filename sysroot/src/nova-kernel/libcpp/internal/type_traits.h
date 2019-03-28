/**
 * @file type_traits.h
 * @author Seth McBee
 * @date 2019-3-21
 */

#pragma once

namespace std
{

template<class T>
struct remove_reference
{
    typedef T type;
};

template<class T>
struct remove_reference<T&>
{
    typedef T type;
};

template<class T>
struct remove_reference<T&&>
{
    typedef T type;
};

} // namespace std
