#include <libc.h>

int strlen(const char *str) {
    int len;
    for (len = 0; *str != '\0'; str++) {
        len++;
    }
    return len;
}

char *strcpy(char *dest, const char *src) {
    do {
        *dest++ = *src++;
    } while (*src != 0);
    return dest;
}

char *strcat(char *dest, const char *src) {
    while (*dest != 0) {
        *dest = *dest++;
    }
    do {
        *dest++ = *src++;
    } while (*src != 0);
    return dest;
}

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for (; count != 0; count--)
        *dp++ = *sp++;
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count) {
    char *temp = (char *)dest;
    for (; count != 0; count--)
        *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
    unsigned short *temp = (unsigned short *)dest;
    for (; count != 0; count--)
        *temp++ = val;
    return dest;
}

int memcmp(const void *dest, const void *src, unsigned int size) {
    const unsigned char* a = (const unsigned char*) dest;
    const unsigned char* b = (const unsigned char*) src;
    for (unsigned int i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

int atoi(const char *str) {
  int num = 0;
  for (int i = 0; str[i] != '\0'; ++i) {
    num = num * 10 + str[i] - '0';
  }
  return num;
}
