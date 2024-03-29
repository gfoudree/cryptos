#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>
#include <screen.h>
#include "multiboot.h"
#include <idt.h>
#include <pci.h>

#define NUM_SYSCALLS 20

typedef struct syscall {
  uint8_t syscall;
  void *handler;
} syscall_t;

typedef struct kernel_data {
  int *p_sysctl;
  syscall_t syscall_table[NUM_SYSCALLS];
  
  tty_t tty;
  multiboot_info_t* mbt;

  uint32_t *heap_ptr;
  uint32_t *heap_base;
  uint32_t *heap_end;

  uint32_t cpu_ticks;
  uint32_t uptime; //In seconds

  uint32_t *page_directory; // Pointer to page directory table

  uint16_t num_pci_devices;
  struct pci_dev *pci_devs;
} kernel_data_t;

extern kernel_data_t kernel_data;

extern void syscall_gate(void);
void init_syscalls();
void syscall_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);
void print_mmap(multiboot_info_t *mbt);

#endif
