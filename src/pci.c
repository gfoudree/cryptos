#include <pci.h>

// This site has some good info on PCI and e1000 drivers: https://github.com/icexin/eggos
#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

void pci_enum_bus() {
    // Count number of PCI devices and malloc array

    for (int bus = 0; bus < 256; bus++) {
        for (int dev = 0; dev < 32; dev++) {
            for (int function = 0; function < 8; function++) {
                uint16_t vendorId = (uint16_t)pci_read_config_word(bus, dev, function, 0);
                if (vendorId != 0xffff) {
                    kernel_data.num_pci_devices++;
                }
            }
        }
    }

    kernel_data.pci_devs = kmalloc(sizeof(pci_dev_t) * (kernel_data.num_pci_devices) + 1);

    // Populate fields
    int index = 0;
    for (int bus = 0; bus < 256; bus++) {
        for (int dev = 0; dev < 32; dev++) {
            for (int function = 0; function < 8; function++) {
                uint16_t vendorId = (uint16_t)pci_read_config_word(bus, dev, function, 0);
                if (vendorId != 0xffff) {
                    populate_pci_dev_struct(bus, dev, function, &kernel_data.pci_devs[index]);
                    index++;
                }
            }
        }
    }
}

void populate_pci_dev_struct(int bus, int dev, int function, pci_dev_t *pci) {
    pci->vendor_id = (uint16_t)pci_read_config_word(bus, dev, function, 0);

    // Get device ID, IRQ line, and class
    pci->device_id = pci_read_config_word(bus, dev, function, 0) >> 16;
    uint16_t pci_class = pci_read_config_word(bus, dev, function, 8) >> 16;
    pci->pci_class = (uint8_t)((pci_class >> 8) & 0xff);
    pci->pci_subclass = (uint8_t)(pci_class & 0xff);

    pci->irq_line = pci_read_config_word(bus, dev, function, 0x3c) & 0xff;
    pci->interrupt_pin = (pci_read_config_word(bus, dev, function, 0x3c) >> 8) & 0xff;

    pci->BAR0 = pci_read_config_word(bus, dev, function, 0x10);
    pci->BAR1 = pci_read_config_word(bus, dev, function, 0x14);
    pci->BAR2 = pci_read_config_word(bus, dev, function, 0x18);
    pci->BAR3 = pci_read_config_word(bus, dev, function, 0x1C);
    pci->BAR4 = pci_read_config_word(bus, dev, function, 0x20);
    pci->BAR5 = pci_read_config_word(bus, dev, function, 0x24);

    pci->expansion_rom = pci_read_config_word(bus, dev, function, 0x30);
}

void pci_dump_dev_struct(pci_dev_t pci) {
    printk("PCI Vendor: ");
    k_printhex(pci.vendor_id);
    printk(" Dev ID: ");
    k_printhex(pci.device_id);
    printk(" Class: ");
    k_printhex(pci.pci_class);
    printk(" Sub Class: ");
    k_printhex(pci.pci_subclass);
    printk(" IRQ Line: ");
    k_printhex(pci.irq_line);
    printk("\n");
}

uint32_t pci_read_config_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    uint32_t addr = (uint32_t)0x80000000 | (uint32_t)(bus << 16) | (device << 11) | (function << 8) | reg;

    // Use port I/O to get PCI info
    outl(PCI_CONFIG_ADDR_PORT, addr);
    return inl(PCI_CONFIG_DATA_PORT);
}