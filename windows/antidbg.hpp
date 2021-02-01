#ifndef ANTIDBG_HPP
#define ANTIDBG_HPP

#include <stdbool.h>
#include <winternl.h>

/* CheckDebuggerPEB()
 *
 *      Checks the state of the Process Environment Block for the
 *      following properties for the presence of a debugger:
 *          - BeingDebugged
 *          - NtGlobalFlag
 *          - ForceFlags and Flags in ProcessHeap
 */
bool CheckDebuggerPEB(void)
{
    // parse out the PEB from FS or GS offsets
    PPEB pPeb;

#ifdef _WIN32
    //pPeb = (PPEB)__readfsdword(0x30);
    __asm__("movl %%fs:0x30, %0" : "=r" (pPeb) : : );
#else
    //pPeb = (PPEB)__readgsqword(0x60);
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
    PVOID pHeap = (PVOID)((PBYTE) pPeb + 0x18);
    DWORD Flags = *(PDWORD)((PBYTE) pHeap + 0x40);
    DWORD ForceFlags = *(PDWORD)((PBYTE) pHeap + 0x44);
#else
    PVOID pHeap = (PVOID)((PBYTE) pPeb + 0x30);
    DWORD Flags = *(PDWORD)((PBYTE pHeap + 0x70);
    DWORD ForceFlags = *(PDWORD)((PBYTE pHeap + 0x74);
#endif

    if (Flags & ~HEAP_GROWABLE || ForceFlags)
        return true;

    // not being debugged!
    return false;
}

/* CheckBreakpoint()
 *
 *      Checks to see if setting a breakpoint is possible, since that can only occur with a 
 *      debugger attached.
 */
bool CheckBreakpoint(void) 
{
    return false;
}

/* CheckTrapFlag()
 *
 *      Check to see if a debugger cleared the trap flag to prevent certain
 *      exceptions from showing up
 */
bool CheckTrapFlag(void)
{
    return false;
}

#endif
