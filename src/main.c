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
		if (inb(0x60) != c)
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

static void sys_int(void) {
	k_printf("OS called!", 0x07);
}

void _kmain()
{
	init_gdt();
	idt_install();
	isrs_install();
	init_video();
	irq_install();

	//keyboard_install();
	//irq_install_handler(1, &keyboard_handler);

	init_serial(COM1);
	p_sysctl  = init_sysctl();

	sysctl(COM_DEBUG, 1);

	idt_set_gate(0x80, (unsigned long int)&sys_int, 0x8, 0x8e); //Setup sys_int handler (int 0x80)

	k_printf("Cryptos ver. 0.02", 0x7);
	k_printf("Initializing video... done", 0x7);
  k_printf("Initializing COM1... done", 0x7);

	sendStr(COM1, "COM1 Port Initialized!");
	__asm__("int $0x80");
	/*
	int i;
	for (i = 0; i < 100; i++) {
		char c[2] = {0};

		c[0] = kbdus[getScancode()];
		k_printf(c, 0x07);
	}
	k_printf("Done!", 0x07);

	*/
	for (;;) {
		__asm__("nop");
	}
}
