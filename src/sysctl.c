#include <sysctl.h>

static int sysctls[2];

void sysctl(const int param, const int value)
{
	sysctls[param] = value;
}

int *init_sysctl()
{
	int i;
	for (i = 0; i < sizeof(sysctls) / sizeof(int); i++)
	{
		sysctls[i] = 0;
	}
	return sysctls;
}
