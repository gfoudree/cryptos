#ifndef PCI_H
#define PCI_H

#include <types.h>
#include <ioports.h>
#include <screen.h>

typedef struct pci_dev {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t pci_class;
    uint8_t pci_subclass;
    uint8_t irq_line;
    uint8_t type;

    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t eprom_bar;

} pci_dev_t;

void pci_enum_bus(pci_dev_t *devs);
uint32_t pci_read_config_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
void pci_dump_dev_struct(pci_dev_t *pci);

#endif