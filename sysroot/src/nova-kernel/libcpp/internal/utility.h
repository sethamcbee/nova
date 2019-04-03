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
constexpr std::add_const_t<T>& as_const(T& t) noexcept
{
    return t;
}

// Rvalue arguments are not allowed.
template <class T>
void as_const(const T&&) = delete;


template <class T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

} // namespace std
