#include <irq.h>
#include <ioports.h>
#include <keyboard.h>
#include <screen.h>

void keyboard_handler(struct regs *r) {
    unsigned char scancode = inb(0x60);
    if (scancode & 0x80) {

    } else {
        putchar((const char)kbdus[scancode]);
    }
}

void keyboard_install(void) {
    irq_install_handler(1, &keyboard_handler);
}
