#include <screen.h>
#include <ioports.h>
#include <gdt.h>
#include <serial.h>
#include <sysctl.h>
#include <idt.h>
#include <irq.h>
#include <isrs.h>
#include <keyboard.h>
#include <cmos.h>
#include <kernel.h>
#include <timer.h>
#include <mm.h>
#include <pci.h>
#include <ata.h>

#include <fs/ff.h>

#include <fs/diskio.h>

kernel_data_t kernel_data;

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            printk(fno.fname);
            printk("printed\n");
            if (res != FR_OK || fno.fname[0] == 0) break;
            
        }
        f_closedir(&dir);
    } else {
        printk("Error opening directory\n");
    }

    return res;
}

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

    pci_enum_bus(&kernel_data.pci_devs);

    unsigned char buf[2048] = {0};
    ATA_init();
    //read_sectors_ATA_PIO(&buf, 0x0, 1);

    //disk_read(1, &buf, 0, 1);
    FATFS fs;
    FRESULT res;
    char buff[256] = {0};

    res = f_mount(&fs, "", 1);
    
    if (res == FR_OK) {
       
        strcpy(buff, "/");
         printk("Mount done\n");
        res = scan_files(buff);
    } else {
        printk("Invalid FS\n");
    }
    
    for (;;) {
        __asm__ volatile ("nop");
    }
}
