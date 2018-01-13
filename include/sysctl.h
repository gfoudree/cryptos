#ifndef SYSCTL_H
#define SYSCTL_H

#define NUM_SYSCTLS 2

// Kernel Settings
#define COM_DEBUG 1

void sysctl(const int param, const int value);
int *init_sysctl();

#endif
