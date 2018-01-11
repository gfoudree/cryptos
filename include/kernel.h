#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>

inline uint64_t _rtdsc();
inline uint32_t _read_cr0();
inline uint32_t _read_cr2();
inline uint32_t _read_cr3();
inline uint32_t _read_cr4();

#endif
