#include <cmos.h>

void read_cmos_rtc(cmos_time_t *time) {
  //Wait if RTC is updating
  do {
    outb(CMOS_ADDR, CMOS_STATUS_REG_A);
  }
  while(inb(CMOS_DATA) & 0x80);

  time->seconds = BCD2BIN(read_cmos_register(0x00));
	time->minutes = BCD2BIN(read_cmos_register(0x02));
	time->hours   = read_cmos_register(0x04);
	time->weekday = BCD2BIN(read_cmos_register(0x06));
	time->day	   = BCD2BIN(read_cmos_register(0x07));
	time->month   = BCD2BIN(read_cmos_register(0x08));
	time->year    = BCD2BIN(read_cmos_register(0x09));
	time->century = BCD2BIN(read_cmos_register(0x32));
	time->registerA = BCD2BIN(read_cmos_register(0x0A));
  time->registerB = BCD2BIN(read_cmos_register(0x0B));

  if (!(time->registerB & 0x04)) {
    time->hours = ((time->hours & 0x7F) + 12) % 24; //Convert to 24 hour time
  }
}

inline uint8_t read_cmos_register(unsigned int reg) {
  outb(CMOS_ADDR, reg);
  return inb(CMOS_DATA);
}
