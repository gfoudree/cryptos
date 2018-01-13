#include <screen.h>
#include <sysctl.h>
#include <serial.h>
#include <ioports.h>
#include <kernel.h>

extern kernel_data_t kernel_data;

void k_cls(void) {
    for (int i = 0; i < COLUMNS*ROWS*2; i = i + 2) {
        *(kernel_data.tty.videoPtr + i) = ' ';
				*(kernel_data.tty.videoPtr + i + 1) = kernel_data.tty.curr_color;
    }
    kernel_data.tty.csr_y = 0;
    kernel_data.tty.csr_x = 0;
}

void putchar(const char c) {
	if (kernel_data.tty.csr_x == COLUMNS || c == '\n') {
		kernel_data.tty.csr_y++;
		kernel_data.tty.csr_x = 0;
	}
  if (kernel_data.tty.csr_y == ROWS) {
    k_cls();
  }

	if (c == '\n') {
		update_cursor(kernel_data.tty.csr_y, kernel_data.tty.csr_x);
		return;
	}
  unsigned int offset = (kernel_data.tty.csr_y * COLUMNS * 2) + kernel_data.tty.csr_x * 2;

  kernel_data.tty.videoPtr[offset] = c;
  kernel_data.tty.videoPtr[offset + 1] = kernel_data.tty.curr_color;
  kernel_data.tty.csr_x++;
  update_cursor(kernel_data.tty.csr_y, kernel_data.tty.csr_x);
}

void printk(const char *message) {
    if (kernel_data.p_sysctl[COM_DEBUG] > 0) { //Check for serial debugging
        sendStr(COM1, message);
    }
    while (*message != '\0') {
			putchar(*message);
			*message++;
    }
}

void k_printdec(unsigned int value) {
    unsigned int temp = value;
    int i = 0, j = 0;
    char chra[32], chrb[32];
    if (value == 0) {
        printk("0");
    }
    while (temp > 0) {
        chra[i] = '0' + temp%10;
        temp /= 10;
        i++;
    }
    chra[i] = 0;
    chrb[i--] = 0;
    while (i >= 0) {
        chrb[i--] = chra[j++];
    }
    printk(chrb);
}

void k_printhex(uint32_t num) {
  char buffer[16] = {0};
  uint8_t len = 0;

  buffer[0] = '0';
  buffer[1] = 'x';

  do {
    buffer[len+2] = hex_table[num & 0xF];
    len++;
    num >>= 4;
  } while (num != 0);

  //Reverse string
  for (int i = 0; i < len / 2; i++) {
    buffer[i+2] ^= buffer[len - i - 1 + 2];
    buffer[len - i - 1 + 2] ^= buffer[i + 2];
    buffer[i + 2] ^= buffer[len - i - 1 + 2];
  }
  printk(buffer);
}

void update_cursor(int row, int col) {
    unsigned short position = (row*80) + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position&0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}

void init_video(void) {
    kernel_data.tty.videoPtr = (unsigned char *)0xB8000;

    kernel_data.tty.csr_x = 0;
    kernel_data.tty.csr_y = 0;
    kernel_data.tty.curr_line = 0;
    kernel_data.tty.curr_color = lGray;

    k_cls();
		update_cursor(kernel_data.tty.csr_y, kernel_data.tty.csr_x);
}

void tty_set_color(unsigned int color) {
    kernel_data.tty.curr_color = color;
}
