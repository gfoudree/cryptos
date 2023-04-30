#include <ata.h>

void ATA_IRQ_handler(struct regs *r)
{
}

void cleanup_ATA_identify_fields(struct ATA_identify *ATA_info) {
    int i;

    // Swap characters for model
    for (i = 0; i < 39; i += 2) {
        char tmp = ATA_info->model[i+1];
        ATA_info->model[i+1] = ATA_info->model[i];
        ATA_info->model[i] = tmp; 
    }
    ATA_info->model[39] = '\0';
}

void ATA_init()
{
    uint16_t ATA_identify_buf[ATA_IDENTIFY_BUF_SIZE] = {0};
    struct ATA_identify *ATA_info = (struct ATA_identify*)ATA_identify_buf;

    irq_install_handler(IRQ_ATA, ATA_IRQ_handler);

    if (!ATA_identify_devs(ATA_identify_buf)) {
        printk("Error initializing ATA devices\n");
    }
    uint32_t drive_size_mb = (ATA_info->sectors_28/2)/1024;

    cleanup_ATA_identify_fields(ATA_info);

    printk("[IDE] Drive ");
    printk(ATA_info->model);
    printk(" Size ");
    k_printdec(drive_size_mb);
    printk("MB\n");
}

int ATA_identify_devs(uint16_t *ATA_buf) {
    ATA_wait_BSY();
    outb(REG_DRIVE, MASTER_DRIVE); // Select master drive

    // Clear other registers
    outb(REG_SECTOR_COUNT, 0);
    outb(REG_SECTOR_NUMBER, 0);
    outb(REG_CYLINDER_LOW, 0);
    outb(REG_CYLINDER_HIGH, 0);

    // Send IDENTIFY command
    outb(REG_COMMAND, CMD_IDENTIFY);

    // Read status
    uint8_t status = inb(REG_STATUS);
    if (status == 0) { // No disk present!
        return 0;
    }
    ATA_wait_BSY();

    if ((inb(REG_CYLINDER_LOW) != 0) || (inb(REG_CYLINDER_HIGH) != 0)) { // Non-ATA disk
        return 0;
    }

    ATA_wait_DRQ();

    status = inb(REG_STATUS);
    if (status & STATUS_ERR) { // Error occured
        return 0;
    }
    
    for (int i = 0; i < 256; i++) {
        ATA_buf[i] = inw(REG_DATA);
    }
    return 1;
}

void read_sectors_ATA_PIO(uint32_t *target_address, uint32_t LBA,
			  uint8_t sector_count)
{
    ATA_wait_BSY();
    outb(REG_DRIVE, 0xE0 | ((LBA >> 24) & 0xF));
    outb(REG_SECTOR_COUNT, sector_count);
    outb(REG_SECTOR_NUMBER, (uint8_t) LBA);
    outb(REG_CYLINDER_LOW, (uint8_t) (LBA >> 8));
    outb(REG_CYLINDER_HIGH, (uint8_t) (LBA >> 16));
    outb(REG_COMMAND, CMD_READ);		// Send the read command

    uint16_t *target = (uint16_t *) target_address;

    for (int j = 0; j < sector_count; j++) {
        ATA_wait_BSY();
        ATA_wait_DRQ();
        for (int i = 0; i < 256; i++)
            target[i] = inw(REG_DATA);
        target += 256;
    }
}

void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count,
			   uint32_t * bytes)
{
    ATA_wait_BSY();
    outb(REG_DRIVE, 0xE0 | ((LBA >> 24) & 0xF));
    outb(REG_SECTOR_COUNT, sector_count);
    outb(REG_SECTOR_NUMBER, (uint8_t) LBA);
    outb(REG_CYLINDER_LOW, (uint8_t) (LBA >> 8));
    outb(REG_CYLINDER_HIGH, (uint8_t) (LBA >> 16));
    outb(REG_COMMAND, CMD_WRITE);		// Send the write command

    for (int j = 0; j < sector_count; j++) {
        ATA_wait_BSY();
        ATA_wait_DRQ();
        for (int i = 0; i < 256; i++) 
            outl(REG_DATA, bytes[i]);
    }
}

// Block while BSY bit is set
inline void ATA_wait_BSY()
{
    uint8_t status = inb(0x1F7);
    while ((status & STATUS_BSY) == 1) {
	status = inb(0x1F7);
    }
}

// Block until DRQ bit is set
inline void ATA_wait_DRQ()
{
    uint8_t status = inb(0x1F7);
    while ((status & STATUS_RDY) == 0) {
	status = inb(0x1F7);
    }
}
