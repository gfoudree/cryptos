#include <libc.h>
#include <idt.h>

static struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) -1;
    idtp.base = (int)&idt;

    memset((void*)&idt, 0, sizeof(struct idt_entry) * 256); //Clear out structure

    idt_load();
}
