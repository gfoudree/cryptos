#include <mm.h>
#include <kernel.h>

void *kmalloc(uint32_t sz) {
    void *ret = 0;

    // Return null if we are requesting 0 bytes
    if (!(sz > 0)) {
      return 0;
    }
    
    // Try and find an open chunk of size sz, iterate over chunks
    void *it = kernel_data.heap_base;
    while (it != kernel_data.heap_ptr) {
        memory_block_t *metadata = (memory_block_t*)(it);
        if (metadata->free && metadata->size >= sz) {
            // Found available chunk!
            metadata->free = 0;

            // Set returned pointer to point to memory after metadata block
            ret = it + sizeof(memory_block_t);

            return ret;
        }
        // Compute next node pointer
        it = (it + sizeof(memory_block_t) + metadata->size);
    }

    // Build metadata
    memory_block_t mb = {0};
    mb.free = 0;
    mb.size = sz;

    memcpy(kernel_data.heap_ptr, &mb, sizeof(memory_block_t));
    
    // Set returned pointer to point to memory after metadata block
    ret = kernel_data.heap_ptr + sizeof(memory_block_t);

    // Update head_ptr to point to the end of the allocated buffer
    kernel_data.heap_ptr += (sz + sizeof(memory_block_t));

    // Return pointer to memory
    return ret;
}

void kfree(void *p) {
    // Get pointer to metadata structure
    memory_block_t *metadata = (memory_block_t*)(p - sizeof(memory_block_t));
    metadata->free = 1; // Mark it free

    // Check if we are at the end of the memory region
    if ((p + metadata->size) == kernel_data.heap_ptr) {
        // Rewind head pointer
        kernel_data.heap_ptr = (kernel_data.heap_ptr - (sizeof(memory_block_t) + metadata->size));
    }
}

void page_translate(uint32_t virt, uint32_t phys, uint16_t flags, uint32_t *page_directory) {
  uint32_t page_index = (virt >> 22); //Index into page table

  uint32_t page_entry = 0;
  page_entry |= (PAGE_4MB | PAGE_WRITABLE | PAGE_PRESENT | flags); //Set flags, 4MB set as default
  page_entry |= (phys & 0xFFF00000); //0xFFF masks bits 31..22 of address for PTE
  page_directory[page_index] = page_entry;

  // Need to invalidate page table
  __asm__ volatile("invlpg (%0)" :: "b"(virt) : "memory");
  //__asm__ volatile("mov %0, %%cr3":: "r"(p));

}

//Have to pass in page_directory pointer because we cannot use relocated addresses!
void page_translate_noreloc(uint32_t virt, uint32_t phys, uint16_t flags, uint32_t page_directory_ptr[]) {
  uint16_t page_index = virt >> 22; //Index into page table

  uint32_t page_entry = 0;
  page_entry |= (PAGE_4MB | flags); //Set flags, 4MB set as default
  page_entry |= (phys & 0xFFF00000); //0xFFF masks bits 31..22 of address for PTE
  page_directory_ptr[page_index] = page_entry;
}

void setup_memory() {
    // Read page table directory pointer from cr3
    uint32_t cr3 = 0;
    __asm__ volatile("movl %%cr3, %0" :"=r"(cr3) : :);
    kernel_data.page_directory = (uint32_t*)cr3;

    // Dump TLB with ctrl+alt+shift+2 -> QEMU monitor mode. Then type: "info tlb"
    // Setup user-space page mapping
    page_translate(0x80000000, 0x00C00000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80400000, 0x01000000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80800000, 0x01400000, PAGE_USER, kernel_data.page_directory);
    page_translate(0x80c00000, 0x01800000, PAGE_USER, kernel_data.page_directory);

    // Setup kernel heap page mapping
    page_translate(0xC0400000, 0x00400000, 0, kernel_data.page_directory);
    page_translate(0xC0800000, 0x00800000, 0, kernel_data.page_directory);
    page_translate(0xC0C00000, 0x00C00000, 0, kernel_data.page_directory);
    kernel_data.heap_base = (void*)0xC0400000;
    kernel_data.heap_ptr = kernel_data.heap_base;
    kernel_data.heap_end = (void*)0xC1000000;
}