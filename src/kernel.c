#include <kernel.h>

inline uint64_t _rtdsc() {
  uint64_t ret;
  asm volatile("rtdsc" : "=A"(ret));
  return ret;
}

inline uint32_t _read_cr0() {
  uint32_t ret;
  asm volatile("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

inline uint32_t _read_cr2() {
  uint32_t ret;
  asm volatile("mov %%cr2, %0" : "=r"(ret));
  return ret;
}

inline uint32_t _read_cr3() {
  uint32_t ret;
  asm volatile("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

inline uint32_t _read_cr4() {
  uint32_t ret;
  asm volatile("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

void sys_shutdown() {
  asm volatile("hlt");
}

void syscall_handler(unsigned int num) {
  
}

void init_syscalls() {
  for (int i = 0; i < NUM_SYSCALLS; i++) {
    syscall_t call;
    call.syscall = i;
    call.handler = 0;
    syscall_table[i] = call;
  }

  syscall_table[0].handler = &sys_shutdown;
}
