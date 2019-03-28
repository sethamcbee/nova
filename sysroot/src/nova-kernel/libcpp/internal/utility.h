/**
 * @file utility.h
 * @author Seth McBee
 * @date 2019-3-21
 */

#pragma once

#include <type_traits>

namespace std
{

template <class T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

} // namespace std
