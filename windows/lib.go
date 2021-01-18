package windows

// #cgo CFLAGS: -g -Wall
// #include "windows.h"
import "C"

// Stub function for checking `BeingDebugged` in Process Environment Block through FFI
// Mitigation: plugin that automatically unsets `BeingDebugged` flag
func CheckBeingDebuggedPEB() bool {
    return bool(C.CheckBeingDebuggedPEB())
}

// Main routine used in detecting debuggers in Windows, calls every known technique in the library
// and returns a bool for later consumption.
func WindowsDebugging() bool {

    // basic: BeingDebugged PEB check
    if CheckBeingDebuggedPEB() {
        return true
    }

    // basic: 

    return false
}
