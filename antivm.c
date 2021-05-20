#include <stdlib.h>
#include "antivm.h"

/* Re-implementation of BSD-style strncmp */
static inline int
inline_strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    do {
        if (*s1 != *s2++)
            return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));

        if (*s1++ == 0)
            break;

    } while (--n != 0);
    return 0;
}

static inline void
cpuid(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    asm volatile("cpuid"
        : "=a" (*eax),
          "=b" (*ebx),
          "=c" (*ecx),
          "=d" (*edx)
        : "0" (*eax), "2" (*ecx));
}


bool CheckCPUIDHypervisor(void)
{
    char name[13];
    name[12] = 0;

    unsigned int eax = 0x40000000;
    cpuid(&eax, (unsigned int *) &name[0], (unsigned int *) &name[8], (unsigned int *) &name[4]);

    // check against set of hypervisor strings
    char hypervisors[3][12] = {"XenVMMXenVMM", "VMwareVMware", "KVMKVMKVM"};
    for (size_t i = 0; i < 3; i++) {
        if (inline_strncmp(name, hypervisors[i], 12) == 0)
            return true;
    }
    return false;
}

bool CheckCPUIDIsVM(void)
{
    unsigned eax = 0x1;
    unsigned int *ecx;
    cpuid(&eax, NULL, ecx, NULL);
    if ((*ecx & (1 << 31)) >> 31)
        return true;

    return false;
}
