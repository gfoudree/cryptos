#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#include <libc.h>

void init_serial(int serialPort);
void sendByte(int serialPort, unsigned char b);
void sendStr(int serialPort, const char *str);
char recvByte(int serialPort);
void recvStr(int serialPort, char *str, const int len);
