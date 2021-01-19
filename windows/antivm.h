#ifndef ANTIVM_H
#define ANTIVM_H

#include <stdbool.h>

#if defined(WIN32) || defined(WIN64)

#include <windows.h>
#include <winternl.h>

bool CheckCPUIDHypervisor(void)
{
    __asm {
        xor ecx, ecx

        ; call cpuid with eax = 1
        mov eax, 1
        cpuid

        ; get 31st bit in ecx
        bt ecx, 31

        ; store in AL reg for return
        setc al
        retn
    }
}

#endif
#endif
