package windows

// #cgo CFLAGS: -Wall
// #include "antidbg.hpp"
// #include "antivm.hpp"
import "C"

// Wrapper function for checking `BeingDebugged` in Process Environment Block
// Mitigation: plugin that automatically patches PEB
func CheckDebuggerPEB() bool {
    return bool(C.CheckDebuggerPEB())
}

// Wrapper function for checking if adding breakpoint is possible
func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}
