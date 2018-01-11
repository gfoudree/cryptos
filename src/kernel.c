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
