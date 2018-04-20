#ifndef MM_H
#define MM_H

#include <kernel.h>

extern kernel_data_t kernel_data;


uint32_t kmalloc(uint32_t sz);

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

extern uint32_t page_directory [1024];

/*
typedef struct page {
    uint8_t present : 1;
    uint8_t rw : 1;
    uint8_t user : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t unused : 7;
    uint32_t frame : 20;
} page_t;

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  page_table_t *tables[1024];
  uint32_t tables_phys[1024];
  uint32_t phys_addr;
} page_directory_t;
*/

#endif
