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
using remove_reference_t = typename remove_reference<T>::type;


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
using remove_pointer_t = typename remove_pointer<T>::type;


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

template <class T, size_t N>
struct remove_extent<T[N]>
{
    using type = T;
};

template <class T>
using remove_extent_t = typename remove_extent<T>::type;


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
using remove_const_t = typename remove_const<T>::type;


template <class T>
struct remove_volatile
{
    using type = T;
};

template <class T>
struct remove_volatile<volatile T>
{
    using type = T;
};

template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;


template <class T>
struct remove_cv
{
    using type = remove_volatile_t<remove_const_t<T>>;
};

template <class T>
using remove_cv_t = typename remove_cv<T>::type;


template <class T>
struct remove_cvref
{
    using type = remove_cv<remove_reference<T>>;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;


template <class T>
struct add_const
{
    using type = const T;
};

template <class T>
using add_const_t = typename add_const<T>::type;


template <class T>
struct add_volatile
{
    using type = volatile T;
};

template <class T>
using add_volatile_t = typename add_volatile<T>::type;


template <class T>
struct add_cv
{
    using type = add_const_t<add_volatile_t<T>>;
};

template <class T>
using add_cv_t = typename add_cv<T>::type;


template <class T, class U>
struct is_same : public false_type {};

template <class T>
struct is_same<T, T> : public true_type {};

template <class T, class U>
constexpr auto is_same_v = is_same<T, U>::value;


template <class T>
struct is_array : public false_type {};

template <class T>
struct is_array<T[]> : public true_type {};

template <class T, size_t N>
struct is_array<T[N]> : public true_type {};

template <class T>
constexpr auto is_array_v = is_array<T>::value;


template <class T, class Enable = void>
struct is_void : public false_type {};

template <class T>
struct is_void
<
    T,
    enable_if_t
    <
        is_same_v
        <
            T,
            void
        >
    >
> : public true_type {};

template <class T>
constexpr auto is_void_v = is_void<T>::value;


template <class T, class Enable = void>
struct is_null_pointer : public false_type {};

template <class T>
struct is_null_pointer
<
    T,
    enable_if_t
    <
        is_same_v
        <
            T,
            nullptr_t
        >
    >
> : public true_type {};

template <class T>
constexpr auto is_null_pointer_v = is_null_pointer<T>::value;


template <class T>
struct is_const : public false_type {};

template <class T>
struct is_const<const T> : public true_type {};

template <class T>
constexpr auto is_const_v = is_const<T>::value;


template <class T>
struct is_volatile : public false_type {};

template <class T>
struct is_volatile<volatile T> : public true_type {};

template <class T>
constexpr auto is_volatile_v = is_volatile<T>::value;


template <class T, class Enable = void>
struct is_abstract : public false_type {};

template <class T>
struct is_abstract
<
    T,
    enable_if_t
    <
        __is_abstract(T)
    >
> : public true_type {};

template <class T>
constexpr auto is_abstract_v = is_abstract<T>::value;


template <class T, class Enable = void>
struct decay
{
    using type = remove_cvref_t<T>;
};

template <class T>
struct decay
<
    T,
    enable_if_t
    <
        is_array_v<T>
    >
>
{
    using type = remove_extent_t<remove_cvref_t<T>>*;
};

template <class T>
using decay_t = typename decay<T>::type;

} // namespace std
