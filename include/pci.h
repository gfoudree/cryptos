#ifndef PCI_H
#define PCI_H

#include <types.h>
#include <ioports.h>
#include <screen.h>
#include <mm.h>
#include <kernel.h>


typedef struct pci_dev {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t pci_class;
    uint8_t pci_subclass;
    uint8_t irq_line;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t expansion_rom;
    uint8_t interrupt_pin;
} pci_dev_t;

void pci_enum_bus();
uint32_t pci_read_config_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
void populate_pci_dev_struct(int bus, int dev, int function, pci_dev_t *pci);
void pci_dump_dev_struct(pci_dev_t pci);

#endif