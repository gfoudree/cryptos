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

void page_translate(uint32_t virt, uint32_t phys, uint16_t flags) {
  uint16_t page_index = virt >> 22; //Index into page table

  uint32_t page_entry = 0;
  page_entry |= (PAGE_4MB | flags); //Set flags, 4MB set as default
  page_entry |= (phys & 0xFFF00000); //0xFFF masks bits 31..22 of address for PTE
  page_directory[page_index] = page_entry;
}

//Have to pass in page_directory pointer because we cannot use relocated addresses!
void page_translate_noreloc(uint32_t virt, uint32_t phys, uint16_t flags, uint32_t page_directory_ptr[]) {
  uint16_t page_index = virt >> 22; //Index into page table

  uint32_t page_entry = 0;
  page_entry |= (PAGE_4MB | flags); //Set flags, 4MB set as default
  page_entry |= (phys & 0xFFF00000); //0xFFF masks bits 31..22 of address for PTE
  page_directory_ptr[page_index] = page_entry;
}
