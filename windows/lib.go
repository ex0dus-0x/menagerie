package windows

// #cgo CFLAGS: -g -Wall -fexceptions
// #include "antidbg.h"
// #include "antivm.h"
import "C"

// Stub function for checking `BeingDebugged` in Process Environment Block
// Mitigation: plugin that automatically mutates PEB
func CheckBeingDebuggedPEB() bool {
    return bool(C.CheckBeingDebuggedPEB())
}

// Stub function for checking `NtGlobalFlags` in Process Environment Block
// Mitigation: plugin that automatically mutates PEB
func CheckNtGlobalFlagPEB() bool {
    return bool(C.CheckNtGlobalFlagPEB())
}

// Stub function for checking if adding breakpoint is possible
func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}
