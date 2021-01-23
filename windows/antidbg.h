#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdbool.h>

#if _WIN32 || __WIN64

#include <windows.h>
#include <winternl.h>
#include <excpt.h>

/* CheckBeingDebuggedPEB() - debug flags
 *
 *      Checks to see if the BeingDebugged attribute is set in the executable's
 *      Process Environment Block to check for debugging.
 */
bool CheckBeingDebuggedPEB(void)
{
#ifndef _WIN64
    PPEB pPeb = (PPEB)__readfsdword(0x30);
#else
    PPEB pPeb = (PPEB)__readgsqword(0x60);
#endif // _WIN64

    if (pPeb->BeingDebugged)
        return true;
    else
        return false;
}

/* CheckNtGlobalFlagPEB() - debug flags
 *
 *      Checks for NtGlobalFlag to be 0x70 to represent that the debugger
 *      forked off a debugee process
 */
bool CheckNtGlobalFlagPEB(void)
{
#ifndef _WIN64
    PPEB pPeb = (PPEB)__readfsdword(0x30);
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0x68);
#else
    PPEB pPeb = (PPEB)__readgsqword(0x60);
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0xbc);
#endif // _WIN64

    if (dwNtGlobalFlag & 0x70)
        return true;
    else
        return false;
}

/* CheckBreakpoint() - exceptions
 *
 *      Checks to see if setting a breakpoint is possible, since that can only occur with a 
 *      debugger attached.
 */
bool CheckBreakpoint(void) 
{
    __try {
        __asm { int 3 };
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

/* CheckTrapFlag() - exceptions
 *
 *      Check to see if a debugger cleared the trap flag to prevent certain
 *      exceptions from showing up
 */
bool CheckTrapFlag(void)
{
    __try {
        __asm {
            pushfd
            mov dword ptr [esp], 0x100
            popfd
            nop
        }
        return true;
    } __except (EXCEPTION_SINGLE_STEP) {
        return false;
    }
}

#else

/* Fill-in for non-Windows platforms */
bool CheckBeingDebuggedPEB(void)
{
    return false;
}

/* Fill-in for non-Windows platforms */
bool CheckNtGlobalFlagPEB(void)
{
    return false;
}

/* Fill-in for non-Windows platforms */
bool CheckBreakpoint(void)
{
    return false;
}

/* Fill-in for non-Windows platforms */
bool CheckTrapFlag(void)
{
    return false;
}

#endif
#endif
