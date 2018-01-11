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
    k_printf("OS called!");
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

    asm volatile("sti"); //Enable interrupts
    
    sysctl(COM_DEBUG, 1);

    idt_set_gate(0x80, (unsigned long int)&sys_int, 0x8, 0x8e); //Setup sys_int handler (int 0x80)

    k_printf("Cryptos ver. 0.02\n");
    k_printf("Initializing video... done\n");
    k_printf("Initializing COM1... done\n");

    sendStr(COM1, "COM1 Port Initialized!\n");


    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbt->mmap_addr;
    while (mmap < mbt->mmap_addr + mbt->mmap_length) {
        switch (mmap->type) {
        case 1:
            k_printf("Available\n");
            break;
        case 2:
            k_printf("Reserved\n");
            break;
        case 3:
            k_printf("ACPI Reserved\n");
            break;
        }
        mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
    }

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
