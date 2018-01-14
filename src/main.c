#include <screen.h>
#include <ioports.h>
#include <gdt.h>
#include <serial.h>
#include <sysctl.h>
#include <idt.h>
#include <irq.h>
#include <isrs.h>
#include <keyboard.h>
#include <multiboot.h>
#include <cmos.h>
#include <kernel.h>
#include <timer.h>

kernel_data_t kernel_data;

void _kmain(multiboot_info_t* mbt, unsigned int magic) {
    kernel_data.mbt = mbt;
    __asm__ volatile("movl %0, %%eax" : "=r"(kernel_data.heap_ptr) : :);

    init_gdt();
    idt_install();

    irq_install();
    isrs_install();
    keyboard_install();
    //timer_install();

    init_video();
    printk("Cryptos ver. 0.03\n");
    printk("Initializing Video... done\n");

    init_serial(COM1);
    printk("Initializing COM1... done\n");
    sendStr(COM1, "COM1 Port Initialized!\n");

    kernel_data.p_sysctl = init_sysctl();

    init_syscalls();
    __asm__ volatile("sti"); //Enable interrupts

    sysctl(COM_DEBUG, 1);

    print_mmap(mbt);

    __asm__("mov $0x80, %eax");
    __asm__("int $0x80");
    for (;;) {
        __asm__ volatile ("nop");
    }
}
