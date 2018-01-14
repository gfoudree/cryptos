#ifndef TIMER_H
#define TIMER_H

#include <irq.h>
#include <types.h>
#include <ioports.h>
#include <kernel.h>

extern kernel_data_t kernel_data;

#define PIT_CH0_DATA_PORT 0x40
#define PIT_CH1_DATA_PORT 0x41
#define PIT_CH2_DATA_PORT 0x42
#define PIT_CMD_REGISTER 0x43

void timer_install();
void timer_sleep(uint32_t ticks);
void timer_handler(regs_t *registers);

#endif
