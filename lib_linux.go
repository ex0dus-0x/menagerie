package menagerie

// #cgo CFLAGS: -Icommon
// #cgo LDFLAGS: -lsystemd
// #include "linux/antidbg.h"
// #include "linux/antivm.h"
// #include "common/cpuid.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

// Main routine to call to execute all known debugger detection heuristics.
//  - Ptrace
//  - Breakpoint Handler Detection
//  - Procfs Fingerprinting
func AntiDebugging() bool {
    return CheckProcessFingerprint() || CheckBreakpoint() || CheckBasicPtrace() || CheckProcessHeapRelocate()
}

//// WRAPPERS ////

// Most basic Linux anti-debugging call with PTRACE_TRACEME.
func CheckBasicPtrace() bool {
    return bool(C.CheckBasicPtrace())
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

// Checks to see if the debugger has relocated the heap segment elsewhere by allocating space
// and checking memory offset.
func CheckProcessHeapRelocate() bool {
    return bool(C.CheckProcessHeapRelocate())
}

// TODO: check parent name with readlink

/*=========================== ANTI-SANDBOXING ===========================*/

// Main routine to call to execute all known sandbox/VM detection heuristics.
func AntiVM() bool {
    return CheckCPUID()
}

//// WRAPPERS ////

func CheckCPUID() bool {
    return bool(C.CheckCPUID())
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Injects tricky inlined assembly at any point of execution to confuse disassembler
// and emit confusing disassembly.
func AntiDisassembly() {
}
