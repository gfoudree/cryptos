#include <irq.h>
#include <ioports.h>
#include <keyboard.h>
#include <screen.h>

void keyboard_handler(struct regs *r) {
    unsigned char scancode = inb(KEYBD_OUTCMD_REG);
    if ((scancode & 0x80) == 128) { //Key released because bit 0x80 set
      scancode &= (~0x80); //Clear released bit
      if (scancode == L_SHIFT || scancode == R_SHIFT) {
        shift_pressed = 0; //Release shift
      }
    }
    else { //Key pressed
        if (scancode == L_SHIFT || scancode == R_SHIFT)
          shift_pressed = 1;
        else {
          int key = kbdus[scancode];
          if (shift_pressed == 1 && key > 96 && key < 123) {
              key -= 32; //Convert to caps
          }
            putchar((const char)key);
        }
    }
}

void keyboard_install(void) {
  keyboard_setleds(0, 0, 1);
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_send_cmd(int cmd, unsigned char data) {
  while ((inb(KEYBD_STATS_REG) & 0x2) != 0) {} //Wait for keyboard to be ready
  outb(KEYBD_OUTCMD_REG, cmd);
  while ((inb(KEYBD_STATS_REG) & 0x2) != 0) {} //Wait for keyboard to be ready
  outb(KEYBD_OUTCMD_REG, data);
}

void keyboard_setleds(int scrollLock, int capsLock, int numLock) {
    int data = 0;
    if (scrollLock)
        data |= 1 << 0;
    if(numLock)
        data |= 1 << 1;
    if (capsLock)
        data |= 1 << 2;

    keyboard_send_cmd(KEYBD_CMD_SETLED, data);
}
