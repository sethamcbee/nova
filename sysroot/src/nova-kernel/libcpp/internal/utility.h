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
constexpr add_const_t<T>& as_const(T& t) noexcept
{
    return t;
}

// Rvalue arguments are not allowed.
template <class T>
void as_const(const T&&) = delete;


template <class T>
remove_reference_t<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}


template <class T>
constexpr T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(remove_reference_t<T>&& t) noexcept
{
    return static_cast<T&&>(t);
}


template <class T>
constexpr void swap(T& t1, T& t2) noexcept
{
    T tmp(move(t1));
    t1 = move(t2);
    t2 = move(tmp);
}

template <class T, size_t N>
constexpr void swap(T (&t1)[N], T (&t2)[N]) noexcept
{
    for (size_t i = 0; i < N; ++i)
    {
        swap(t1[i], t2[i]);
    }
}


template <class T1, class T2 = T1>
constexpr T1 exchange(T1& t1, T2&& t2) noexcept
{
    T1 tmp(std::move(t1));
    t1 = std::forward<T2>(t2);
    return tmp;
}


template <class T1, class T2>
struct pair
{
    using first_type = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    constexpr pair();

    pair(const T1& t1, const T2& t2)
        : first(t1), second(t2) {}

    template <class U1, class U2>
    constexpr pair(U1&& u1, U2&& u2)
        : first(forward<U1>(u1)), second(forward<U2>(u2)) {}

    pair(const pair& other) = default;
    pair(pair&& other) = default;

    constexpr pair& operator=(const pair& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <class U1, class U2>
    constexpr pair& operator=(const pair<U1, U2>& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    constexpr pair& operator=(pair&& other) noexcept
    {
        first = move(other.first);
        second = move(other.second);
        return *this;
    }

    template <class U1, class U2>
    constexpr pair& operator=(pair<U1, U2>&& other) noexcept
    {
        first = forward<U1>(other.first);
        second = forward<U2>(other.second);
        return *this;
    }

    constexpr void swap(pair& other) noexcept
    {
        using std::swap;
        swap(first, other.first);
        swap(second, other.second);
    }
};

template <class T1, class T2>
pair(T1, T2) -> pair<T1, T2>;

template <class T1, class T2>
constexpr void swap(pair<T1, T2>& p1, pair<T1, T2>& p2) noexcept
{
    p1.swap(p2);
}

template <class T1, class T2>
constexpr pair<T1, T2> make_pair(T1&& t1, T2&& t2)
{
    return {t1, t2};
}


template <size_t I, class T1, class T2>
constexpr T1& get(pair<T1, T2>& p)
{
    if constexpr (I == 0)
    {
        return p.first;
    }
    else if constexpr (I == 1)
    {
        return p.second;
    }
    else
    {
        static_assert(I < 2, "Out-of-range std::get(std::pair)");
    }
}

} // namespace std
