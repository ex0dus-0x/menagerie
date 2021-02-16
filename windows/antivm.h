#ifndef ANTIVM_H
#define ANTIVM_H

#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

// TODO
bool CheckCPUIDHypervisor(void)
{
    uint32_t eax, ebx, ecx, edx;
    eax = 0x40000000;

    asm volatile("cpuid"
        : "=a" (*eax),
          "=b" (*ebx),
          "=c" (*ecx),
          "=d" (*edx)
        : "0" (*eax), "2" (*ecx));

    return false;
}

#endif
