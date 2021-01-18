#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdbool.h>

#if defined(WIN32)

#include <Windows.h>

/* CheckBeingDebuggedPEB() - anti-DBG
 *
 *      Checks to see if the BeingDebugged attribute is set in the executable's
 *      Process Environment Block to check for debugging.
 */
bool CheckBeingDebuggedPEB(void)
{
    bool isDebugged = false;
    _asm {
		xor eax, eax;			
		mov eax, fs:[0x30];         // segment register FS
		mov eax, [eax + 0x02];      // BeingDebugged is offset PEB + 2
		and eax, 0x000000FF;
		mov isDebuggged, eax;
	}
    return isDebugged;
}


bool CheckProcessHeapPEB(void)
{
    bool isDebugged = false;
    _asm {
        xor eax, eax;
        mov eax, fs:[0x30];
        mov eax, [eax + 0x18];
    }
    return isDebugged
}

#else

/* Fill-in for non-Windows platforms */
bool CheckBeingDebuggedPEB(void)
{
    return false;
}

#endif
#endif
