/**
 * @file gcc_support.cc
 * @author Seth McBee
 * @date 2019-3-20
 * @brief GCC support functions for C++.
 */

extern "C" void kernel_panic(const char*);

extern "C" void __cxa_pure_virtual()
{
    kernel_panic("Pure virtual function call!");
}

void* __dso_handle;

extern "C" int __cxa_atexit(void (*destructor)(void*), void* arg, void* dso)
{
    // STUB.
    return 0;
}
