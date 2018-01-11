#ifndef IRQ_H
#define IRQ_H

//PIC works by getting input from a device then signaling the processor with an interrupt
//There are 2 PICs, master & slave. Master is IRQ 0-7, slave = 8-15

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI 0x20

typedef struct regs
{
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int gs, fs, es, ds;
    unsigned int int_no, err_code;
	  unsigned int eip, cs, eflags, useresp, ss;
} regs_t;

extern void _irq0(void);
extern void _irq1(void);
extern void _irq2(void);
extern void _irq3(void);
extern void _irq4(void);
extern void _irq5(void);
extern void _irq6(void);
extern void _irq7(void);
extern void _irq8(void);
extern void _irq9(void);
extern void _irq10(void);
extern void _irq11(void);
extern void _irq12(void);
extern void _irq13(void);
extern void _irq14(void);
extern void _irq15(void);

extern void disable_pic(void);

void irq_install(void);
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_set_mask(unsigned char irq_line);
void irq_clear_mask(unsigned char irq_line);

#endif
