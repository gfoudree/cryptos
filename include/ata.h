#ifndef ATA_H
#define ATA_H

#include <ioports.h>
#include <types.h>

void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes);
void ATA_init();
int ATA_identify();

#endif