/**
 * @file cmos.cc
 * @author Seth McBee
 * @date 2019-4-3
 */

#include <globals.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/devices/cmos.h>

uint16_t rtc_sec_init;
uint16_t rtc_min_init;
uint16_t rtc_hour_init;
uint8_t rtc_weekday_init;
uint8_t rtc_day_init;
uint8_t rtc_mon_init;
uint8_t rtc_year_init;

// CMOS IO ports.
static constexpr uint16_t CMOS_CMD = 0x70;
static constexpr uint16_t CMOS_DATA = 0x71;

// CMOS command structure.
static constexpr uint8_t NMI_DISABLE_BIT = 7;

// RTC registers.
static constexpr uint8_t RTC_SEC = 0x00;
static constexpr uint8_t RTC_MIN = 0x02;
static constexpr uint8_t RTC_HOUR = 0x04;
static constexpr uint8_t RTC_WEEKDAY = 0x06;
static constexpr uint8_t RTC_DAY = 0x07;
static constexpr uint8_t RTC_MON = 0x08;
static constexpr uint8_t RTC_YEAR = 0x09;
static constexpr uint8_t RTC_CENTURY = 0x32;
static constexpr uint8_t RTC_SR_A = 0x0A;
static constexpr uint8_t RTC_SR_B = 0x0B;

// RTC Status Register A structure.
static constexpr uint8_t RTC_UPDATE_BIT = 7;

// RTC Status Register B structure.
static constexpr uint8_t HOUR_24_BIT = 1;
static constexpr uint8_t BINARY_BIT = 2;

uint8_t cmos_read(uint8_t reg)
{
    cpu_outb(reg, CMOS_CMD);
    cpu_io_wait();
    uint8_t ret = cpu_inb(CMOS_DATA);
    cpu_io_wait();
    return ret;
}

void cmos_write(uint8_t val, uint8_t reg)
{
    cpu_outb(reg, CMOS_CMD);
    cpu_io_wait();
    cpu_outb(val, CMOS_DATA);
    cpu_io_wait();
}

uint8_t bcd_to_bin(uint8_t val)
{
    return ((val / 16) * 10) + (val & 0xF);
}

void cmos_init()
{
    // Wait for RTC update flag to clear once.
    uint8_t status_a = cmos_read(RTC_SR_A);
    bool update_flag = BIT_CHECK(status_a, RTC_UPDATE_BIT);
    while (update_flag == false)
    {
        // Wait for RTC update flag to go high.
        status_a = cmos_read(RTC_SR_A);
        update_flag = BIT_CHECK(status_a, RTC_UPDATE_BIT);
    }
    while (update_flag == true)
    {
        // Wait for RTC update flag to go low.
        status_a = cmos_read(RTC_SR_A);
        update_flag = BIT_CHECK(status_a, RTC_UPDATE_BIT);
    }
    
    uint8_t status_b = cmos_read(RTC_SR_B);
    bool hour_24 = BIT_CHECK(status_b, HOUR_24_BIT);
    bool binary = BIT_CHECK(status_b, BINARY_BIT);
    cpu_outb(hour_24, 0x80);
    cpu_outb(binary, 0x80);
    
    rtc_sec_init = cmos_read(RTC_SEC);
    rtc_min_init = cmos_read(RTC_MIN);
    rtc_hour_init = cmos_read(RTC_HOUR);
    rtc_weekday_init = cmos_read(RTC_WEEKDAY);
    rtc_day_init = cmos_read(RTC_DAY);
    rtc_mon_init = cmos_read(RTC_MON);
    rtc_year_init = cmos_read(RTC_YEAR);
    
    if (!binary)
    {
        rtc_sec_init = bcd_to_bin(rtc_sec_init);
        rtc_min_init = bcd_to_bin(rtc_min_init);
        rtc_hour_init = bcd_to_bin(rtc_hour_init);
        rtc_weekday_init = bcd_to_bin(rtc_weekday_init);
        rtc_day_init = bcd_to_bin(rtc_day_init);
        rtc_mon_init = bcd_to_bin(rtc_mon_init);
        rtc_year_init = bcd_to_bin(rtc_year_init);
    }
}
