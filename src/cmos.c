#include <cmos.h>

void read_cmos_rtc(cmos_time_t *time) {
  //Wait if RTC is updating
  do {
    outb(CMOS_ADDR, CMOS_STATUS_REG_A);
  }
  while(inb(CMOS_DATA) & 0x80);

  time->second = read_cmos_register(0x00);
  time->minute = read_cmos_register(0x02);
  time->hour = read_cmos_register(0x04);
  time->day = read_cmos_register(0x07);
  time->month = read_cmos_register(0x08);
  time->year = read_cmos_register(0x09);
  time->registerB = read_cmos_register(0x0B);

  if (!(time->registerB & 0x04)) { //Convert values
    time->second = (time->second & 0x0F) + ((time->second / 16) * 10);
    time->minute = (time->minute & 0x0F) + ((time->minute / 16) * 10);
    //time->hour = ((time->hour & 0x0F) + (((time->hour & 0x70) / 16) * 10) ) | (time->hour & 0x80);
    time->day = (time->day & 0x0F) + ((time->day / 16) * 10);
    time->month = (time->month & 0x0F) + ((time->month / 16) * 10);
    time->year = (time->year & 0x0F) + ((time->year / 16) * 10);
  }

  if (!(time->registerB & 0x04)) {
    time->hour = ((time->hour & 0x7F) + 12) % 24; //Convert to 24 hour time
  }
}

inline uint8_t read_cmos_register(unsigned int reg) {
  outb(CMOS_ADDR, reg);
  return inb(CMOS_DATA);
}
