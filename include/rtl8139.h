#ifndef RTL8139_H
#define RTL8139_H

// https://wiki.osdev.org/RTL8139

#include <kernel.h>
#include <pci.h>
#include <screen.h>
#include <ioports.h>

#define VENDOR_ID 0x10EC
#define DEVICE_ID 0x8139

typedef struct rtl8139_dev {
    pci_dev_t *pci_dev;
    uint32_t iobase;
    uint32_t mem_base;

    uint8_t mac_addr[6];
} rtl8139_dev_t;

void init_rtl8139(rtl8139_dev_t *rtldev);
void getmac_rtl8139(rtl8139_dev_t *rtldev);

#endif //RTL8139_H
