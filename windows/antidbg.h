#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdbool.h>
#include <winternl.h>

bool CheckDebuggerPEB(void)
{
    // parse out the PEB from FS or GS offsets
    PPEB pPeb;

#ifdef _WIN32
    __asm__("movl %%fs:0x30, %0" : "=r" (pPeb) : : );
#else
    __asm__("movl %%gs:0x60, %0" : "=r" (pPeb) : : );
#endif // _WIN64

    // Do checks one-by-one for complicated control-flow
    // to figure out by reverse engineer

    // check for basic BeingDebugged
    if (pPeb->BeingDebugged)
        return true;

#ifdef _WIN32
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0x68);
#else
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0xbc);
#endif

    // check NtGlobalFlag to be set as 0x70:
    // TODO: annotate the flags that are included to make up 0x70
    if (dwNtGlobalFlag & 0x70)
        return true;

    // parse out pointer to heap from PEB, and two attributes' offsets for antidebug
#ifdef _WIN32
    PVOID pHeap = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x18));
    DWORD Flags = *(PDWORD)((PBYTE) pHeap + 0x40);
    DWORD ForceFlags = *(PDWORD)((PBYTE) pHeap + 0x44);
#else
    PVOID pHeap = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x30));
    DWORD Flags = *(PDWORD)((PBYTE pHeap + 0x70);
    DWORD ForceFlags = *(PDWORD)((PBYTE pHeap + 0x74);
#endif

    if (Flags & ~HEAP_GROWABLE || ForceFlags)
        return true;

    // TODO: Heap Protection

    // not being debugged!
    return false;
}

bool CheckBreakpoints(void)
{
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(GetCurrentThread(), &ctx))
        return false;
    return ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3;
}

#endif
