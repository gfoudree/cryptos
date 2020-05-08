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

kernel_data_t kernel_data;

void _kmain(multiboot_info_t* mbt, uint32_t heap_base) {
    kernel_data.mbt = mbt;

    // Read page table directory pointer from cr3
    uint32_t cr3 = 0;
    __asm__ volatile("movl %%cr3, %0" :"=r"(cr3) : :);
    kernel_data.page_directory = (uint32_t*)cr3;

    // Dump TLB with ctrl+alt+shift+2 -> QEMU monitor mode. Then type: "info tlb"
    // Setup user-space page mapping
    page_translate(0x80000000, 0x00C00000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80400000, 0x01000000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80800000, 0x01400000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80c00000, 0x01800000, PAGE_USER, kernel_data.page_directory);

    // Setup kernel heap page mapping
    page_translate(0xC0400000, 0x00400000, 0, kernel_data.page_directory);
    page_translate(0xC0800000, 0x00800000, 0, kernel_data.page_directory);
    page_translate(0xC0C00000, 0x00C00000, 0, kernel_data.page_directory);
    kernel_data.heap_base = (void*)0xC0400000;
    kernel_data.heap_ptr = kernel_data.heap_base;
    kernel_data.heap_end = (void*)0xC1000000;

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
    strcpy(b, "hello");
    printk(b);
    kfree(b);
    
    __asm__ volatile("sti"); //Enable interrupts
    __asm__("mov $0x80, %eax");
    __asm__("int $0x80");

    for (;;) {
        __asm__ volatile ("nop");
    }
}
