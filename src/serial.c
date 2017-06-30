#include <ioports.h>
#include <serial.h>

void init_serial(int serialPort)
{
        outb(serialPort + 1, 0x00);   //Disable all interrupts
        outb(serialPort + 3, 0x80);   //Set baud rate divisor
        outb(serialPort + 0, 0x03);   //Set divisor to 3 (lo byte) 38400baud
        outb(serialPort + 1, 0x00);   // (hi byte)
        outb(serialPort + 3, 0x03);   //8 bits, not parity, one stop bit
        outb(serialPort + 2, 0xC7);   //Enable FIFO
        outb(serialPort + 4, 0x0B);   //IRQs enabled, RTS/DSR set
}

void sendByte(int serialPort, unsigned char b)
{
	while ((inb(serialPort + 5) & 0x20) == 0)
	{
		//Wait until we are clear to send...
	}

	outb(serialPort, b);
}

void sendStr(int serialPort, const char *str)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		sendByte(serialPort, str[i]);
	}
}

char recvByte(int serialPort)
{
	while ((inb(serialPort + 5) & 1) == 0)
	{
		//Wait for clear to recv
	}
	return inb(serialPort);
}

//Warning: this function is blocking until len# chars are read!
void recvStr(int serialPort, char *str, const int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		str[i] = recvByte(serialPort);
	}
}
