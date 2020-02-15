/**
 * @file new.h
 * @author Seth McBee
 * @date 2019-9-24
 */

#pragma once

// Placement new.

inline void* operator new(size_t, void* p) throw()
{
    return p;
}

inline void* operator new[](size_t, void* p) throw()
{
    return p;
}

inline void operator delete(void*, void*) throw() {}
inline void operator delete[](void*, void*) throw() {}
