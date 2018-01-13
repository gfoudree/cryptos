#ifndef SCREEN_H
#define SCREEN_H

#define COLUMNS 80
#define ROWS 25

#include <types.h>

enum screenColors {
	black = 0x0,
	blue = 0x1,
	green = 0x2,
	cyan = 0x3,
	red = 0x4,
	magenta = 0x5,
	brown = 0x6,
	lGray = 0x7,
	dGray = 0x8,
	lBlue = 0x9,
	lGreen = 0xA,
	lCyan = 0xB,
	lRed = 0xC,
	lMagenta = 0xD,
	yellow = 0xE,
	white = 0xF,
};

typedef struct tty {
	unsigned char *videoPtr;
	unsigned int csr_x;
	unsigned int csr_y;
	unsigned int curr_line;
	unsigned int curr_color;
} tty_t;

int printf(const char* restrict format, ...);
void k_cls(void);
void k_printhex(uint32_t num);
void k_printdec(unsigned int value);
void printk(const char *message);
void init_video(void);
void update_cursor(int row, int col);
void tty_set_color(unsigned int color);
void putchar(const char c);

const static char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

#endif
