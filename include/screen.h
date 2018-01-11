#define COLUMNS 80
#define LINES 24

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

void scroll(unsigned int mode);
void move_csr(void);
void k_cls(void);
unsigned int k_printfEx(const char *message, unsigned int line, unsigned int mode);
void k_printf(const char *message);
void init_video(void);
void update_cursor(int row, int col);
void tty_set_color(unsigned int color);
void putchar(const char c);
static tty_t tty;
