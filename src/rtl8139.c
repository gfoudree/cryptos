#include <rtl8139.h>

void init_rtl8139(rtl8139_dev_t *rtldev) {
    // Find the PCI device if it exists
    for (int i = 0; i < kernel_data.num_pci_devices; i++) {
        pci_dev_t dev = kernel_data.pci_devs[i];

        if (dev.vendor_id == VENDOR_ID && dev.device_id == DEVICE_ID) {
            rtldev->pci_dev = &kernel_data.pci_devs[i];
        }
    }
    rtldev->iobase = rtldev->pci_dev->BAR0 & (~0x3);
    rtldev->mem_base = rtldev->pci_dev->BAR0 & (~0xF);

    // Power on device
    outb(rtldev->iobase + 0x52, 0x0);
    outb(rtldev->iobase + 0x37, 0x10);  //Reset;
    while ((inb(rtldev->iobase + 0x37) & 0x10) != 0) {
    }

    getmac_rtl8139(rtldev);
}

void getmac_rtl8139(rtl8139_dev_t *rtldev) {
    uint32_t mac_1 = inl(rtldev->iobase + 0x00);
    uint32_t mac_2 = inl(rtldev->iobase + 0x04);

    rtldev->mac_addr[0] = mac_1 >> 0;
    rtldev->mac_addr[1] = mac_1 >> 8;
    rtldev->mac_addr[2] = mac_1 >> 16;
    rtldev->mac_addr[3] = mac_1 >> 24;
    rtldev->mac_addr[4] = mac_2 >> 0;
    rtldev->mac_addr[5] = mac_2 >> 8;
}