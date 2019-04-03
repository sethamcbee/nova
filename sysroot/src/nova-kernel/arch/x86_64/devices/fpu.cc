/**
 * @file fpu.cc
 * @author Seth McBee
 * @date 2019-4-3
 */

#include <globals.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/devices/fpu.h>

static constexpr uint64_t CR0_MP_BIT = 1;
static constexpr uint64_t CR0_EM_BIT = 2;
static constexpr uint64_t CR0_TS_BIT = 3;
static constexpr uint64_t CR0_NE_BIT = 5;
static constexpr uint64_t CR4_OSFXSR_BIT = 9;
static constexpr uint64_t CR4_OSXMMEXCPT_BIT = 10;
static constexpr uint64_t CR4_OSXSAVE_BIT = 18;

void fpu_init()
{
    volatile uint64_t new_cr0 = cpu_read_cr0();
    new_cr0 = BIT_SET(new_cr0, CR0_MP_BIT);
    new_cr0 = BIT_CLEAR(new_cr0, CR0_EM_BIT);
    //new_cr0 = BIT_CLEAR(new_cr0, CR0_TS_BIT);
    new_cr0 = BIT_SET(new_cr0, CR0_NE_BIT);
    cpu_write_cr0(new_cr0);
    
    volatile uint64_t new_cr4 = cpu_read_cr4();
    new_cr4 = BIT_SET(new_cr4, CR4_OSFXSR_BIT);
    new_cr4 = BIT_SET(new_cr4, CR4_OSXMMEXCPT_BIT);
    //new_cr4 = BIT_SET(new_cr4, CR4_OSXSAVE_BIT);
    cpu_write_cr4(new_cr4);
    
    asm volatile
    (
        "finit \n"
        :
        :
        : "memory"
    );
    
    alignas(16) uint8_t fxsave_region[512];
    asm volatile
    (
        "fxsave %0 \n"
        :
        : "m" (fxsave_region)
        :
    );
}
