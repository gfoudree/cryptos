#include "e1000.h"


void e1000_write_command(uint32_t phys_addr, uint32_t val) {
    (uint32_t*)(phys_addr) = val;
}

bool e1000_detect_eeprom(pci_dev_t *pci_dev) {

}

bool e1000_init(pci_dev_t *pci_devs) {
    bool return_status = false;
  for (int i = 0; i < MAX_PCI_DEVS; i++) {
    // Check if we have an e1000 NIC
    pci_dev_t *pci_dev = &pci_devs[i];
    if (!(pci_dev->vendor_id == 0x8086 && pci_dev->device_id == 0x100E)) {
      continue;
    } else {
      uint32_t *mac = (uint32_t *)(pci_dev->bar0 + 0x5400);
      return_status = true;
    }
  }
  return return_status;
}