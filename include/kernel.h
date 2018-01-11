#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>

#define NUM_SYSCALLS 20

typedef struct syscall {
  uint8_t syscall;
  void *handler;
} syscall_t;

syscall_t syscall_table[NUM_SYSCALLS];

inline uint64_t _rtdsc();
inline uint32_t _read_cr0();
inline uint32_t _read_cr2();
inline uint32_t _read_cr3();
inline uint32_t _read_cr4();

void init_syscalls();
void syscall_handler();

#endif
