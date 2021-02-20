package menagerie

// #cgo CFLAGS:
// #include "macos/antidbg.h"
// #include "macos/antivm.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

 // Main routine to call to execute all known debugger detection heuristics.
 //  - Ptrace
func AntiDebugging() bool {
    return CheckBasicPtrace()
}

//// WRAPPERS ////

// Defines a basic PT_DENY_ATTACH implementation that dynamically loads 
// ptrace to mitigate detection of symbols in static analysis.
func CheckBasicPtrace() bool {
    return bool(C.CheckBasicPtrace())
}

/*=========================== ANTI-SANDBOXING ===========================*/

func AntiVM() bool {
    return CheckSysctl()
}

//// WRAPPERS ////

// Defines a virtualization check using sysctl
func CheckSysctl() bool {
    return bool(C.CheckSysctl())
}


/*=========================== ANTI-DISASSEMBLY ===========================*/

// Cross-platform function used to inject confusing assembler payloads before sensitive
// functionality to trick any linear or recursive disassembler.
func AntiDisassembly() {
}
