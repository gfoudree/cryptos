#ifndef MM_H
#define MM_H

#include <kernel.h>

extern kernel_data_t kernel_data;
uint32_t kmalloc(uint32_t sz);

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

#endif
