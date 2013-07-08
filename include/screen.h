#define COLUMNS 80
#define LINES 24

void scroll(unsigned int mode);
void move_csr(void);
void k_cls(void);
unsigned int k_printfEx(char *message, unsigned int line, unsigned int mode);
void k_printf(char *message, unsigned int mode);
void init_video(void);
