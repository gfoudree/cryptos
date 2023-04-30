#include <pci.h>

// This site has some good info on PCI and e1000 drivers: https://github.com/icexin/eggos
#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

void pci_enum_bus(pci_dev_t *devs) {
    uint16_t current_dev = 0;

    for (int bus = 0; bus < 256; bus++) {
        for (int dev = 0; dev < 32; dev++) {
            for (int function = 0; function < 8; function++) {
                uint16_t vendorId = (uint16_t)pci_read_config_word(bus, dev, function, 0);
                if (vendorId != 0xffff) {
                    pci_dev_t *pci = &devs[current_dev];
                    pci->vendor_id = vendorId;

                    // Get device ID, IRQ line, and class
                    pci->device_id = pci_read_config_word(bus, dev, function, 0) >> 16;

                    uint16_t pci_class = pci_read_config_word(bus, dev, function, 8) >> 16;
                    pci->pci_class = (uint8_t)((pci_class >> 8) & 0xff);
                    pci->pci_subclass = (uint8_t)(pci_class & 0xff);

                    pci->irq_line = pci_read_config_word(bus, dev, function, 0x3c) & 0xff;


                    // Get the PCI header type
                    pci->type = (pci_read_config_word(bus, dev, function, 0xc) >> 16) & 0xff;
                    if (pci->type == 0x0) {
                        pci->bar0 = pci_read_config_word(bus, dev, function, 0x10);
                        pci->bar1 = pci_read_config_word(bus, dev, function, 0x14);
                        pci->bar2 = pci_read_config_word(bus, dev, function, 0x18);
                        pci->bar3 = pci_read_config_word(bus, dev, function, 0x1c);
                        pci->bar4 = pci_read_config_word(bus, dev, function, 0x20);
                        pci->bar5 = pci_read_config_word(bus, dev, function, 0x24);
                        pci->eprom_bar = pci_read_config_word(bus, dev, function, 0x30);
                    }
                    pci_dump_dev_struct(pci);
                    current_dev++;
                }
            }
        }
    }
}

void pci_dump_dev_struct(pci_dev_t *pci) {
    printk("PCI Vendor: ");
    k_printhex(pci->vendor_id);
    printk(" Dev ID: ");
    k_printhex(pci->device_id);
    printk(" Class: ");
    k_printhex(pci->pci_class);
    printk(" Sub Class: ");
    k_printhex(pci->pci_subclass);
    printk(" IRQ Line: ");
    k_printhex(pci->irq_line);
    printk("\n");
}

uint32_t pci_read_config_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    uint32_t addr = (uint32_t)0x80000000 | (uint32_t)(bus << 16) | (device << 11) | (function << 8) | reg;

    // Use port I/O to get PCI info
    outl(PCI_CONFIG_ADDR_PORT, addr);
    return inl(PCI_CONFIG_DATA_PORT);
}