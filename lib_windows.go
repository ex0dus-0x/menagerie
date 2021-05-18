package menagerie

// #cgo CFLAGS: -Icommon
// #include "windows/antidbg.h"
// #include "windows/antivm.h"
// #include "common/cpuid.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

// Checks the state of the Process Environment Block for the
// following properties for the presence of a debugger:
//   - BeingDebugged
//   - NtGlobalFlag
//   - ForceFlags and Flags in ProcessHeap
func CheckDebuggerPEB() bool {
    return bool(C.CheckDebuggerPEB())
}

func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}

/*=========================== ANTI-SANDBOXING ===========================*/

func CheckCPUIDIsVM() bool {
    return bool(C.CheckCPUIDIsVM())
}

func CheckCPUIDHypervisor() bool {
    return bool(C.CheckCPUIDHypervisor())
}
