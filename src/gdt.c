#include <gdt.h>

extern void gdt_flush(unsigned int gdt);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void init_gdt()
{
   	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   	gdt_ptr.base  = (unsigned int)&gdt_entries;

    //We have to use segmentation, but we want to use paging in the end so just
    //set everything to overlap so we can control things with paging later...
   	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   	gdt_flush((unsigned int)&gdt_ptr);
}

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}
