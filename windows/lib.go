package windows

// #cgo CFLAGS: -Wall
// #include "antidbg.h"
// #include "antivm.h"
import "C"

// Wrapper function for checking `BeingDebugged` in Process Environment Block
// Mitigation: plugin that automatically patches PEB
func CheckBeingDebuggedPEB() bool {
    return bool(C.CheckBeingDebuggedPEB())
}

// Wrapper function for checking `NtGlobalFlags` in Process Environment Block
// Mitigation: plugin that automatically patches PEB
func CheckNtGlobalFlagPEB() bool {
    return bool(C.CheckNtGlobalFlagPEB())
}

// Wrapper function for checking if adding breakpoint is possible
func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}
