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

void _kmain()
{
	init_gdt();
	idt_install();
	isrs_install();
	irq_install();
	init_video();
	keyboard_install();
	init_serial(COM1);
	p_sysctl  = init_sysctl();
	
	sysctl(COM_DEBUG, 1);
	
	k_printf("Serial Initialized!", 0x7);
	k_printf("Cryptos ver. 0.01", 0x7);
	k_printf("Initializing video... done", 0x7);

	sendStr(COM1, "COM1 Port Initialized!");

	__asm__("hlt");
	for (;;);
}

