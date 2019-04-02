/**
 * @file type_traits.h
 * @author Seth McBee
 * @date 2019-3-21
 */

#pragma once

namespace std
{

template <class T, T v>
struct integral_constant
{
    using type = integral_constant<T, v>;
    using value_type = T;
    
    static constexpr value_type value = v;
    
    constexpr operator value_type() const noexcept
    {
        return v;
    }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;


template <bool Cond, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T>
{
    using type = T;
};

template <bool Cond, class T = void>
using enable_if_t = typename enable_if<Cond, T>::type;


template <bool Cond, class T, class F>
struct conditional {};

template <class T, class F>
struct conditional<true, T, F>
{
    using type = T;
};

template <class T, class F>
struct conditional<false, T, F>
{
    using type = F;
};

template <bool Cond, class T, class F>
using conditional_t = typename conditional<Cond, T, F>::type;


template <class T>
struct remove_reference
{
    using type = T;
};

template <class T>
struct remove_reference<T&>
{
    using type = T;
};

template <class T>
struct remove_reference<T&&>
{
    using type = T;
};

template <class T>
using remove_reference_t = remove_reference<T>;


template <class T>
struct remove_pointer
{
    using type = T;
};

template <class T>
struct remove_pointer<T*>
{
    using type = T;
};

template <class T>
using remove_pointer_t = remove_pointer<T>;


template <class T>
struct remove_extent
{
    using type = T;
};

template <class T>
struct remove_extent<T[]>
{
    using type = T;
};

template <class T>
using remove_extent_t = remove_extent<T>;


template <class T>
struct remove_const
{
    using type = T;
};

template <class T>
struct remove_const<const T>
{
    using type = T;
};

template <class T>
using remove_const_t = remove_const<T>;


template <class T, class U>
struct is_same : public false_type {};

template <class T>
struct is_same<T, T> : public true_type {};

template <class T, class U>
constexpr auto is_same_v = is_same<T, U>::value;


template <class T>
struct is_abstract
{
    using value_type = bool;
    
    static constexpr value_type value = __is_abstract(T);
    
    using type = integral_constant<bool, value>;
    
    constexpr operator value_type() const noexcept
    {
        return value;
    }
};

template <class T>
constexpr auto is_abstract_v = is_abstract<T>::value;

} // namespace std
