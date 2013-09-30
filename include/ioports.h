#ifndef IOPORTS_H
#define IOPORTS_H

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);

#endif
