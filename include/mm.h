#ifndef MM_H
#define MM_H

#include <kernel.h>

#define PAGE_PRESENT 1
#define PAGE_WRITABLE 2
#define PAGE_USER 4
#define PAGE_WRITE_THROUGH 8
#define PAGE_CACHE_DISABLE 16
#define PAGE_ACCESSED 32
#define PAGE_DIRTY 64
#define PAGE_4MB 128

extern kernel_data_t kernel_data;
extern uint32_t page_directory [1024];

void page_translate(uint32_t virt, uint32_t phys, uint16_t flags, uint32_t p[]);
void page_translate_noreloc(uint32_t virt, uint32_t phys, uint16_t flags, uint32_t page_directory_ptr[]);
void *kmalloc(uint32_t sz);
void kfree(void *p);

typedef struct __attribute__((__packed__)) memory_block {
    uint8_t free;
    uint32_t size;
} memory_block_t;

/*
typedef struct __attribute__((__packed__)) page_directory {
  uint8_t present : 1;
  uint8_t writable : 1;
  uint8_t user : 1;
  uint8_t write_through : 1;
  uint8_t cache_disable : 1;
  uint8_t accessed : 1;
  uint8_t dirty : 1;
  uint8_t page_4mb : 1;

  uint8_t reserved : 1;
  uint8_t available : 3;
  uint16_t reserved_addr : 10;
  uint16_t phys_addr : 10;

  uint32_t remain : 24;
} page_directory_t;
*/

#endif
