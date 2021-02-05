package ems

// #cgo CFLAGS:
// #include "macos/antidbg.h"
// #include "macos/antivm.h"
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

// Defines a basic PT_DENY_ATTACH implementation that dynamically loads 
// ptrace to mitigate detection of symbols in static analysis.
func CheckStealthyPtrace() bool {
    return bool(C.CheckStealthyPtrace())
}

/*=========================== ANTI-SANDBOXING ===========================*/

func AntiSandbox() bool {

    // check sysctl configurations
    if macos.CheckHardwareFingerprint() {
        return true
    }
    return false
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Cross-platform function used to inject confusing assembler payloads before sensitive
// functionality to trick any linear or recursive disassembler.
func AntiDisassembly() {
}
