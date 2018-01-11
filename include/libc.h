#ifndef LIBC_H
#define LIBC_H

int strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
unsigned char *memset(unsigned char *dest, unsigned char val, int count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
int memcmp(const void *dest, const void *src, unsigned int size);
int atoi(const char *str);

#endif
