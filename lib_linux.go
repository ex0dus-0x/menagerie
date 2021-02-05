package ems

// #cgo LDFLAGS: -Wl,--no-as-needed -ldl
// #include "linux/antidbg.h"
// #include "linux/antivm.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

// Main routine to call to execute all known debugger detection heuristics.
//  - Stealthy Ptrace
func AntiDebugging() bool {

    // stealthy but still basic ptrace check
    if CheckStealthyPtrace() == true {
        return true
    }

    return false
}

//// WRAPPERS ////

// Defines a basic antidebug implementation that dynamically loads ptrace
// from libc to mitigate the detection of symbols in static analysis
func CheckStealthyPtrace() bool {
    return bool(C.CheckStealthyPtrace())
}

/*=========================== ANTI-SANDBOXING ===========================*/

// Main routine to call to execute all known sandbox/VM detection heuristics.
func AntiSandbox() bool {
    return false
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Injects tricky inlined assembly at any point of execution to confuse disassembler
// and emit confusing disassembly.
func AntiDisassembly() {
}
