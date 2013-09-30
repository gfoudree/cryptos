#define COLUMNS 80
#define LINES 24

void scroll(unsigned int mode);
void move_csr(void);
void k_cls(void);
unsigned int k_printfEx(char *message, unsigned int line, unsigned int mode);
void k_printf(char *message, unsigned int mode);
void init_video(void);
void update_cursor(int row, int col);

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
