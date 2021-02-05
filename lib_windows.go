package ems

// #cgo CFLAGS:
// #include "windows/antidbg.h"
// #include "windows/antivm.h"
// #include "windows/antidisass.h"
import "C"


/*=========================== ANTI-DEBUGGING ===========================*/

// Main routine to call to execute all known debugger detection heuristics.
//  - Process Environment Block
//  - Breakpoint Detection
func AntiDebugging() bool {

    // basic PEB fingerprinting check
    if CheckDebuggerPEB() == true {
        return true
    }

    // checks to see if breakpoint has been placed
    if CheckBreakpoint() == true {
        return true
    }

    return false
}

// Runs anti-debug detection, and run a callback routine if debugging is detected.
func AntiDebuggingCb(cb func()) {
    if AntiDebugging() == true {
        cb()
    }
}

//// WRAPPERS ////

func CheckDebuggerPEB() bool {
    return bool(C.CheckDebuggerPEB())
}

func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}

/*=========================== ANTI-SANDBOXING ===========================*/

// Main routine to call to execute all known sandbox/VM detection heuristics.
//  - CPUID hypervisor check
func AntiSandbox() bool {

    // check with classic `cpuid` instruction
    if CheckCPUIDHypervisor() == true {
        return true
    }

    return false
}

// Runs cross-platform anti-sandbox detection, and run a callback routine if debugging is detected.
func AntiSandboxCb(cb func()) {
    if AntiSandbox() == true {
        cb()
    }
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
