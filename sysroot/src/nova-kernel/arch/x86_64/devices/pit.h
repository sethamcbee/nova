/**
 * @file pit.h
 * @author Seth McBee
 * @date 2019-4-2
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern uint64_t irq_pit_count;

// Hz.
// min = 18.2065 Hz, max = 1193182 Hz
extern uint32_t PIT_BASE_FREQ;

// Hz.
extern uint32_t PIT_TARGET_FREQ;

extern uint16_t PIT_DIVISOR;

// Hz.
extern float PIT_REAL_FREQ;

void pit_init();

void pit_info();

#ifdef __cplusplus
}
#endif
