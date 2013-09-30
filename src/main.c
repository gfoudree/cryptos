#include <screen.h>
#include <ioports.h>
#include <gdt.h>

void _kmain()
{
	init_video();
	init_gdt();
	k_printf("Cryptos ver. 0.01", 0x7);
	k_printf("Initializing video... done", 0x7);

	__asm__("int $3");
	__asm__("movl $1, %edx");
	for (;;);
}

