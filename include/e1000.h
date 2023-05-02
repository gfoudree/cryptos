#ifndef E1000
#define E1000

#include <pci.h>
#include <types.h>
#include <kernel.h>

bool e1000_init(pci_dev_t *pci_devs);

#endif