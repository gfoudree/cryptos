#include <timer.h>

void timer_install() {
  irq_install_handler(IRQ_TIMER, &timer_handler);

  uint32_t prescaler = 1193180 / 50;
  outb(PIT_CMD_REGISTER, 0x36);
  outb(PIT_CH0_DATA_PORT, prescaler & 0xFF);
  outb(PIT_CH0_DATA_PORT, (prescaler >> 8) & 0xFF);

  kernel_data.cpu_ticks = 0;
  kernel_data.uptime = 0;
}

void timer_sleep(uint32_t ticks) {

}

void timer_handler(regs_t *registers) {
  kernel_data.cpu_ticks++;
  if (kernel_data.cpu_ticks % 18 == 0) {
    kernel_data.uptime++;
  }
}
