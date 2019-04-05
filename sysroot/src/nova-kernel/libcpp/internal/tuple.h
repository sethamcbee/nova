/**
 * @file tuple.h
 * @author Seth McBee
 * @date 2019-4-5
 */

#pragma once

#include <utility>

namespace std
{

template <class... Types>
struct tuple;

// Base tuple.
template <>
struct tuple<> {};

// Recursive tuple.
template <class T, class... Rest>
struct tuple<T, Rest...> : tuple<Rest...>
{
    T data;

    constexpr tuple() {}

    constexpr tuple(const T& t, const Rest&... rest)
        : tuple<Rest...>(rest...), data(t) {}

    constexpr tuple(T&& t, Rest&&... rest)
        : tuple<Rest...>(forward<Rest...>(rest)...),
        data(forward<T>(t)) {}

    constexpr tuple(const tuple& other) = default;

    constexpr tuple(tuple&& other)
        : tuple<Rest...>(move(other)),
        data(move(other.data)) {}

    constexpr tuple& operator=(const tuple& other)
    {
        tuple<Rest...>::operator=(other);
        data = other.data;
        return *this;
    }

    constexpr tuple& operator=(tuple&& other)
    {
        tuple<Rest...>::operator=(std::move(other));
        data = std::move(other.data);
        return *this;
    }

    template <class U, class... UTypes>
    constexpr tuple& operator=(const tuple<U, UTypes...>& other);
};

// Deduction guides.
template <class... Types>
tuple(Types...) -> tuple<Types...>;


template <size_t I, class T>
struct tuple_element;

// Base element.
template <class T, class... Rest>
struct tuple_element<0, tuple<T, Rest...>>
{
    using type = T;
    using TupleType = tuple<T, Rest...>;
};

// Recursive element.
template <size_t I, class T, class... Rest>
struct tuple_element<I, tuple<T, Rest...>>
    : tuple_element<I-1, tuple<Rest...>> {};

template <size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;


// Get the Ith element of a tuple.
template <size_t I, class... Types>
constexpr tuple_element_t<I, tuple<Types...>>& get(tuple<Types...>& t)
{
    using TupleType = typename tuple_element<I, tuple<Types...>>::TupleType;
    return static_cast<TupleType&>(t).data;
}

template <size_t I, class... Types>
constexpr tuple_element_t<I, tuple<Types...>>&& get(tuple<Types...>&& t)
{
    using TupleType = typename tuple_element<I, tuple<Types...>>::TupleType;
    return static_cast<TupleType&&>(t).data;
}


struct Ignore
{
    template <class T>
    const Ignore& operator=(const T&) const
    {
        return *this;
    }
};
inline constexpr Ignore ignore;


template <class... Types>
tuple<Types&...> tie(Types&... args) noexcept
{
    return tuple<Types&...>(args...);
}

}
