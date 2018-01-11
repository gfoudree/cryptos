#include <irq.h>
#include <screen.h>
#include <idt.h>
#include <ioports.h>

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(regs_t *r)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_remap(void) { //Need to remap IRQs to higher level interrupts in the PIC
    outb(PIC1, 0x11);
    outb(PIC2, 0x11);
    outb(PIC1_DATA, PIC1);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);
}

void irq_install(void) {
    irq_remap();

    idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

void irq_handler(regs_t *r) {
    unsigned int irq = r->int_no - 32;
    void (*handler)(regs_t *r) = irq_routines[irq];
    if (handler) {
        handler(r);
    }

    //Send End of Interrupt signal to the PIC
    if (irq >= 8) { //If this came from the slave (IRQ >= 8), must inform both
        outb(PIC2, PIC_EOI);
    }
    outb(PIC1, PIC_EOI);
}

void irq_set_mask(unsigned char irq_line) {
  unsigned int port;
  if (irq_line < 8) { //Talk to master PIC
    port = PIC1_DATA;
  }
  else { //Talk to slave
    irq_line -= 8;
    port = PIC2_DATA;
  }

  uint8_t mask_val = inb(port);
  outb(port, mask_val | (1 << irq_line)); //Set mask bit
}

void irq_clear_mask(unsigned char irq_line) {
  unsigned int port;
  if (irq_line < 8) { //Talk to master PIC
    port = PIC1_DATA;
  }
  else { //Talk to slave
    irq_line -= 8;
    port = PIC2_DATA;
  }

  uint8_t mask_val = inb(port);
  outb(port, mask_val & ~(1 << irq_line)); //Clear mask
}
