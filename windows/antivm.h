#ifndef ANTIVM_H
#define ANTIVM_H

#include <stdbool.h>
#include <cpuid.h>

bool CheckCPUIDHypervisor(void)
{
    // TODO
    int ecx;
    __asm__ (
        "movl %1, eax\n\t"
        "cpuid\n\t"
        "movl %0, eax\n\t"
        "movl %1, ebx\n\t"
        "movl %2, ecx\n\t"
        "movl %3, edx\n\t"
        : "=r" (ecx)
    );
    return false;
}

#endif
