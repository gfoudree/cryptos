#include <mm.h>
#include <kernel.h>

uint32_t kmalloc(uint32_t sz) {
  if (kernel_data.heap_ptr & 0xFFFFF000) { //If the addr is not 4096 aligned
    kernel_data.heap_ptr &= 0xFFFFF000; //Align addr
    kernel_data.heap_ptr += 0x1000; //Increment to next block
  }
  uint32_t mem_addr = kernel_data.heap_ptr;
  kernel_data.heap_ptr += sz;
  return mem_addr;
}
