struct regs
{
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int gs, fs, es, ds;
    unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

void irq_install(void);
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
