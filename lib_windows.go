package ems

// #cgo CFLAGS:
// #include "windows/antidbg.h"
// #include "windows/antivm.h"
// #include "windows/antidisass.h"
import "C"


/*=========================== ANTI-DEBUGGING ===========================*/

// Main routine to call to execute all known debugger detection heuristics.
//  - Process Environment Block
//  - Breakpoint Check
func AntiDebugging() bool {
    return CheckDebuggerPEB()
}

//// WRAPPERS ////

// Checks the state of the Process Environment Block for the
// following properties for the presence of a debugger:
//   - BeingDebugged
//   - NtGlobalFlag
//   - ForceFlags and Flags in ProcessHeap
func CheckDebuggerPEB() bool {
    return bool(C.CheckDebuggerPEB())
}

/*=========================== ANTI-SANDBOXING ===========================*/

// Main routine to call to execute all known sandbox/VM detection heuristics.
//  - CPUID hypervisor check
func AntiSandbox() bool {
    return CheckCPUIDHypervisor()
}

//// WRAPPERS ////

func CheckCPUIDHypervisor() bool {
    return bool(C.CheckCPUIDHypervisor())
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Injects tricky inlined assembly at any point of execution to confuse disassembler
// and emit confusing disassembly.
func AntiDisassembly() {
    C.AntiDisassembly()
}
