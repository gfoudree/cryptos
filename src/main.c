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

int *p_sysctl;

char getScancode() {
    char c = 0;
    do {
        if (inb(0x60) != c) {
            c = inb(0x60);
            if(c > 0)
                return c;
        }
    } while(1);
}

static void sys_int(void) {
    printk("OS called!");
}

static void *setup_heap(void) { //Heap ptr is passed in EAX register
    unsigned long ptr;
    __asm__ volatile("movl %0, %%eax" : "=r"(ptr) : :);
    return (void*)ptr;
}

void _kmain(multiboot_info_t* mbt, unsigned int magic) {
    void *heap_ptr;
    heap_ptr = setup_heap();
    init_gdt();
    idt_install();

    irq_install();
    isrs_install();
    keyboard_install();
    init_video();

    init_serial(COM1);
    p_sysctl  = init_sysctl();

    init_syscalls();
    asm volatile("sti"); //Enable interrupts

    sysctl(COM_DEBUG, 1);

    //idt_set_gate(0x80, (unsigned long int)&sys_int, 0x8, 0x8e); //Setup sys_int handler (int 0x80)

    printk("Cryptos ver. 0.03\n");
    printk("Initializing Video... done\n");
    printk("Initializing COM1... done\n");
    k_printhex(0xcdef);
    sendStr(COM1, "COM1 Port Initialized!\n");

    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbt->mmap_addr;
    while (mmap < mbt->mmap_addr + mbt->mmap_length) {
        switch (mmap->type) {
        case 1:
            k_printdec(mmap->addr);
            printk("Available\n");
            break;
        case 2:
            k_printdec(mmap->addr);
            printk("Reserved\n");
            break;
        case 3:
            printk("ACPI Reserved\n");
            break;
        case 4:
            printk("NVS\n");
            break;
        case 5:
            printk("Bad RAM\n");
            break;
        }
        mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
    }

    cmos_time_t time;
    read_cmos_rtc(&time);

    for (;;) {
        asm volatile ("nop");
    }
}
