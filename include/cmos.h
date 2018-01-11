#ifndef CMOS_H
#define CMOS_H

//Good CMOS memory map here http://www.bioscentral.com/misc/cmosmap.htm

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71
#define CMOS_STATUS_REG_A 0xA
#define CMOS_STATUS_REG_B 0xB
#define CMOS_STATUS_REG_C 0xC
#define CMOS_HDD_REG 0x12

#include <types.h>
#include <ioports.h>

typedef struct cmos_time {
    uint8_t century;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t last_century;
    uint8_t registerB;
} cmos_time_t;

void read_cmos_rtc(cmos_time_t *time);
static inline uint8_t read_cmos_register(unsigned int reg);

#endif
