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
#include <mm.h>
#include <pci.h>
#include <ata.h>

kernel_data_t kernel_data;

void _kmain(multiboot_info_t* mbt, uint32_t heap_base) {
    kernel_data.mbt = mbt;

    setup_memory();
    init_gdt();
    idt_install();
    irq_install();
    isrs_install();
    keyboard_install();
    timer_install();

    init_video();
    printk("Cryptos ver. 0.03\n");
    printk("Initializing Video... done\n");

    init_serial(COM1);
    printk("Initializing COM1... done\n");
    sendStr(COM1, "COM1 Port Initialized!\n");

    kernel_data.p_sysctl = init_sysctl();

    init_syscalls();

    sysctl(COM_DEBUG, 1);

    k_printdec((mbt->mem_lower + mbt->mem_upper)/1000);
    printk(" MB of RAM available\nMemory Map:\n");
    print_mmap(mbt);

    char *b = kmalloc(25);
    strcpy(b, "Boot complete!\n");
    printk(b);
    kfree(b);
    
    __asm__ volatile("sti"); //Enable interrupts
    __asm__("mov $0x80, %eax");
    __asm__("int $0x80");

    pci_enum_bus();

    char buf[2048] = {0};
    ATA_init();
    read_sectors_ATA_PIO(&buf, 0x0, 1);
    for (;;) {
        __asm__ volatile ("nop");
    }
}
