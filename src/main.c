#include <screen.h>

void _kmain()
{
	init_video();
	unsigned char *vidPtr = (unsigned char*) 0xB8000;
	vidPtr[0] = 'H';
	vidPtr[1] = 0x7;
	k_printf("hello world!", 0x7);
	return;
}

