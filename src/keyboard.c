#include <irq.h>
#include <ioports.h>
#include <keyboard.h>

void keyboard_handler(struct regs *r) {
    k_printf("KEY!", 0x0f);
    unsigned char scancode;
    scancode = inb(0x60);
    if (scancode & 0x80) {

    } else {
        k_printf(kbdus[scancode], 0x0f);
    }
}

void keyboard_send_cmd(int cmd) {
    while(1) { //Wait for keyboard buffer to be clear
        if ((keyboard_getstatus() & KEYBD_STATUS_INBUF) == 0)
            break;
    }
    outb(KEYBD_CTRL_OUTCMD, cmd);
}

void keyboard_encoder_send_cmd(int cmd) {
    while(1) {
        if ((keyboard_getstatus() & KEYBD_STATUS_INBUF) == 0)
            break;
    }
    outb(KEYBD_ENC_OUTCMD, cmd);
}

int keyboard_encoder_read_buf() {
    return inb(KEYBD_ENC_INBUF);
}

int keyboard_getstatus(void) {
    return inb(KEYBD_CTRL_STATUS);
}

void keyboard_setleds(bool scrollLock, bool capsLock, bool numLock) {
    int data = 0;
    if (scrollLock)
        data |= 1 << 0;
    if(numLock)
        data |= 1 << 1;
    if (capsLock)
        data |= 1 << 2;

    keyboard_encoder_send_cmd(KEYBD_CMD_SETLED);
    keyboard_encoder_send_cmd(data);
}

bool keyboard_self_test() {
    keyboard_send_cmd(KEYBD_CMD_SELFTEST);
    while(1)
        if (keyboard_getstatus() & KEYBD_STATUS_OUTBUF)
            break;
    if (keyboard_encoder_read_buf() == 0x55)
        return TRUE;
    else
        return FALSE;
}

void reboot_computer() {
    keyboard_encoder_send_cmd(0xFE);
}

void keyboard_install(void) {
    keyboard_setleds(FALSE, FALSE, TRUE);
    irq_install_handler(1, &keyboard_handler);
}
