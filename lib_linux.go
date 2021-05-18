package menagerie

// #cgo CFLAGS: -Icommon
// #cgo LDFLAGS: -lsystemd
// #include "linux/antidbg.h"
// #include "linux/antivm.h"
// #include "common/cpuid.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

// Most basic Linux anti-debugging call with PTRACE_TRACEME.
func CheckDebuggerBasic() bool {
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
func CheckMemoryFingerprint() bool {
    return bool(C.CheckProcessHeapRelocate())
}

/*=========================== ANTI-SANDBOXING ===========================*/

func CheckCPUIDIsVM() bool {
    return bool(C.CheckCPUIDIsVM())
}

func CheckCPUIDHypervisor() bool {
    return bool(C.CheckCPUIDHypervisor())
}
