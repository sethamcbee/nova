/**
 * @file new.cc
 * @author Seth McBee
 * @date 2019-3-20
 */

#include <stddef.h>

extern "C" void* malloc(size_t size);
extern "C" void free(void* p);

void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
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
