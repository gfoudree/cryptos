#include <screen.h>
#include <ioports.h>
#include <gdt.h>
#include <serial.h>
#include <sysctl.h>
#include <idt.h>
#include <irq.h>
#include <isrs.h>
#include <keyboard.h>

int *p_sysctl;

char getScancode()
{
	char c = 0;
	do {
		if (inb(0x60) ! = c)
		{
			c = inb(0x60);
			if(c > 0)
				return c;
		}
	} while(1);
}

static void keyboard_handler(struct regs *r) {
	k_printf("Gotta key bby!", 0x07);
}

void _kmain()
{
	init_gdt();
	idt_install();
	isrs_install();
	init_video();
	irq_install();

	//keyboard_install();
	irq_install_handler(1, &keyboard_handler);
	init_serial(COM1);
	p_sysctl  = init_sysctl();

	sysctl(COM_DEBUG, 1);

	k_printf("Cryptos ver. 0.02", 0x7);
	k_printf("Initializing video... done", 0x7);
  k_printf("Initializing COM1... done", 0x7);

	sendStr(COM1, "COM1 Port Initialized!");

	int i;
	for (i = 0; i < 100; i++) {
		char c[2] = {0};

		c[0] = kbdus[getScancode()];
		k_printf(c, 0x07);
	}
	k_printf("DOne!", 0x07);


	for (;;) {
		__asm__("hlt");
	}
}
