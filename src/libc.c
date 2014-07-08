#include <libc.h>

int strlen(const char *str)
{
	int len;
	for (len = 0; *str != '\0'; str++)
	{
		len++;
	}
	return len;
}

char *strcpy(char *dest, const char *src)
{
	do
	{
		*dest++ = *src++;
	}
	while (*src != 0);
	return dest;
}

char *strcat(char *dest, const char *src)
{
	while (*dest != 0)
	{
		*dest = *dest++;
	}

	do
	{
		*dest++ = *src++;
	}
	while (*src != 0);
	return dest;
}

