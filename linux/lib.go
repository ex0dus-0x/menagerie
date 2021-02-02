package linux

// #cgo LDFLAGS: -Wl,--no-as-needed -ldl
// #include "antidbg.h"
import "C"

// Wrapper function for deploying basic dynamically loaded `ptrace` detection.
// Mitigation: Noop out instructions from manual analysis
func CheckStealthyPtrace() bool {
    return bool(C.CheckPtrace())
}
