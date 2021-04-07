#ifndef ATA_H
#define ATA_H

#include <ioports.h>
#include <types.h>
#include <irq.h>
#include <types.h>
#include <screen.h>

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08		// Set when device has data ready to transfer
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

#define REG_DATA 0x1F0
#define REG_SECTOR_COUNT 0x1F2
#define REG_SECTOR_NUMBER 0x1F3
#define REG_CYLINDER_LOW 0x1F4
#define REG_CYLINDER_HIGH 0x1F5
#define REG_DRIVE 0x1F6
#define REG_COMMAND 0x1F7
#define REG_STATUS 0x1F7

#define MASTER_DRIVE 0xA0
#define SLAVE_DRIVE 0xB0

#define CMD_IDENTIFY 0xEC
#define CMD_READ 0x20
#define CMD_WRITE 0x30

#define ATA_IDENTIFY_BUF_SIZE 256

struct ATA_identify {
    uint16_t flags;
    uint16_t unused1[9];
    char serial[20];
    uint16_t unused2[3];
    char firmware[8];
    char model[40];
    uint16_t sectors_per_int;
    uint16_t unused3;
    uint16_t capabilities[2];
    uint16_t unused4[2];
    uint16_t valid_ext_data;
    uint16_t unused5[5];
    uint16_t size_of_rw_mult;
    uint32_t sectors_28;
    uint16_t unused6[38];
    uint64_t sectors_48;
    uint16_t unused7[152];
} __attribute__((packed));

void read_sectors_ATA_PIO(uint32_t *target_address, uint32_t LBA,
                          uint8_t sector_count);
void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t *bytes);
void ATA_init();
int ATA_identify();
static void ATA_wait_BSY();
static void ATA_wait_DRQ();
void cleanup_ATA_identify_fields(struct ATA_identify *ATA_info);
void ATA_IRQ_handler(struct regs *r);

#endif