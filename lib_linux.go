package menagerie

// #cgo LDFLAGS: -Wl,--no-as-needed -ldl -lc
// #include "linux/antidbg.h"
// #include "linux/antivm.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

// Main routine to call to execute all known debugger detection heuristics.
//  - Stealthy Ptrace
//  - Breakpoint Handler Detection
//  - Procfs Fingerprinting
func AntiDebugging() bool {
    return CheckStealthyPtrace() || CheckBreakpoint() || CheckProcessFingerprint()
}

//// WRAPPERS ////

// Basic anti-debug that dynamically loads ptrace to mitigate the detection 
// of symbols in static analysis.
func CheckStealthyPtrace() bool {
    return bool(C.CheckStealthyPtrace())
}

// Detects the presence of a debug by attempting to evoke a debugging SIGTRAP handler 
// when setting a breakpoint.
func CheckBreakpoint() bool {
    return bool(C.CheckBreakpoint())
}

// Parses procfs metadata to fingerprint a tracer process for the current executable.
func CheckProcessFingerprint() bool {
    return bool(C.CheckProcessFingerprint())
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
