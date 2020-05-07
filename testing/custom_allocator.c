#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct __attribute__((__packed__)) memory_block {
    uint8_t free;
    uint32_t size;

} memory_block_t;

void *heap_base;
void *head_ptr;

void *kmalloc(uint32_t sz) {
    void *ret = NULL;

    // Try and find an open chunk of size sz, iterate over chunks
    void *it = heap_base;
    while (it != head_ptr) {
        memory_block_t *metadata = (memory_block_t*)(it);
        if (metadata->free && metadata->size >= sz) {
            // Found available chunk!

            metadata->free = 0;

            // Set returned pointer to point to memory after metadata block
            ret = it + sizeof(memory_block_t);

            printf("Found free chunk @ %p with size %d\n", ret, metadata->size);
            return ret;
        }
        // Compute next node pointer
        it = (it + sizeof(memory_block_t) + metadata->size);
    }

    // Build metadata
    memory_block_t mb = {0};
    mb.free = 0;
    mb.size = sz;

    memcpy(head_ptr, &mb, sizeof(memory_block_t));
    
    // Set returned pointer to point to memory after metadata block
    ret = head_ptr + sizeof(memory_block_t);

    // Update head_ptr to point to the end of the allocated buffer
    head_ptr += (sz + sizeof(memory_block_t));

    // Return pointer to memory
    return ret;
}

void kfree(void *p) {
    // Get pointer to metadata structure
    memory_block_t *metadata = (memory_block_t*)(p - sizeof(memory_block_t));
    metadata->free = 1; // Mark it free
    printf("Freeing %d bytes\n", metadata->size);

    // Check if we are at the end of the memory region
    if ((p + metadata->size) == head_ptr) {
        // Rewind head pointer
        head_ptr = (head_ptr - (sizeof(memory_block_t) + metadata->size));
    }
}

int main() {
    heap_base = malloc(4096);
    head_ptr = heap_base;

    void *p = kmalloc(25);
    void *d = kmalloc(10);
    void *unused = kmalloc(10);

    strcpy(p, "hello");
    strcpy(d, "world");

    kfree(d);

    void *c = kmalloc(5);
    strcpy(c, "dogs");
    kfree(c);
    kfree(p);
    kfree(unused);

    free(heap_base);
}