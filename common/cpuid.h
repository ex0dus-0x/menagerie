/*
 * cpuid.h
 * 
 *      Cross-plaform support for CPUID profiling. Used for hypervisor detection in
 *      Unix and MinGW Windows.
 */
#include "deps.h"

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


bool CheckCPUID(void)
{
    // first check: hypervisor vendor
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

    // second check: flag
    eax = 0x1;
    unsigned int *ebx, *ebx, *ecx, *edx;
    cpuid(&eax, ebx, ecx, edx);
    if ((ecx & (1 << 31)) >> 31)
        return true;

    return false;
}
