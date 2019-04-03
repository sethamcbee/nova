/**
 * @file cmos.h
 * @author Seth McBee
 * @date 2019-4-3
 */

#pragma once

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

void cmos_init();

extern uint16_t rtc_sec_init;
extern uint16_t rtc_min_init;
extern uint16_t rtc_hour_init;
extern uint8_t rtc_weekday_init;
extern uint8_t rtc_day_init;
extern uint8_t rtc_mon_init;
extern uint8_t rtc_year_init;

#ifdef __cplusplus
}
#endif
