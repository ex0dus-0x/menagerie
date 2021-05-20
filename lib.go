package menagerie

// #cgo CFLAGS: -Ilib
// #include "antidbg.h"
// #include "antivm.h"
import "C"

/*=========================== ANTI-DEBUGGING ===========================*/

func CheckDebuggerBasic() bool {
    return bool(C.CheckDebuggerBasic())
}

func ThrowBreakpointExcept() bool {
    return bool(C.ThrowBreakpointExcept())
}

func BreakpointChecksumAt() bool {
    return bool(C.BreakpointChecksumAt())
}

func CheckMemoryFingerprint() bool {
    return bool(C.CheckMemoryFingerprint())
}

func CheckParentTracer() bool {
    return bool(C.CheckParentTracer())
}

/*=========================== ANTI-VM ===========================*/

func CheckCPUIDIsVM() bool {
    return bool(C.CheckCPUIDIsVM())
}

func CheckCPUIDHypervisor() bool {
    return bool(C.CheckCPUIDHypervisor())
}

/*=========================== ANTI-SANDBOX ===========================*/
