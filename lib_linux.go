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

// Runs cross-platform anti-debug detection, and run a callback routine if debugging is detected.
func AntiDebuggingCb(cb func()) {
    if AntiDebugging() == true {
        cb()
    }
}

//// WRAPPERS ////

func CheckStealthyPtrace() bool {
    return bool(C.CheckStealthyPtrace())
}

/*=========================== ANTI-SANDBOXING ===========================*/

// Main routine to call to execute all known sandbox/VM detection heuristics.
func AntiSandbox() bool {
    return false
}

// Runs cross-platform anti-sandbox detection, and run a callback routine if debugging is detected.
func AntiSandboxCb(cb func()) {
    if AntiSandbox() == true {
        cb()
    }
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Injects tricky inlined assembly at any point of execution to confuse disassembler
// and emit confusing disassembly.
func AntiDisassembly() {
}
