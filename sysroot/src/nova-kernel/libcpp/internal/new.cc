/**
 * @file new.cc
 * @author Seth McBee
 * @date 2019-3-20
 */

#include <stddef.h>

#include <stdlib.h>

void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void* operator new(size_t size, void* p)
{
    return p;
}

void* operator new[](size_t size, void* p)
{
    return p;
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, size_t size)
{
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, size_t n)
{
    free(p);
}

void operator delete(void*, void*) {}
void operator delete[](void*, void*) {}
