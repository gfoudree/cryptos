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

void map(uint32_t phys, uint32_t virt, uint32_t flags, uint32_t *pd) {
    uint32_t page_index = (virt >> 22);
    uint32_t page_entry = 0;
    
    page_entry |= (PAGE_4MB | PAGE_WRITABLE | PAGE_PRESENT | flags);
    page_entry |= (phys & 0xFFF00000);

    pd[page_index] = page_entry;

   __asm__ volatile("invlpg (%0)" :: "b"(virt) : "memory");
}

void _kmain(multiboot_info_t* mbt, uint32_t heap_base) {
    kernel_data.mbt = mbt;
    kernel_data.heap_base = heap_base;
    kernel_data.heap_ptr = kernel_data.heap_base;

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

    __asm__ volatile("sti"); //Enable interrupts
    __asm__("mov $0x80, %eax");
    __asm__("int $0x80");

    for (;;) {
        __asm__ volatile ("nop");
    }
}
