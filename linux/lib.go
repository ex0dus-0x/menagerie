package linux

import "syscall"

// Simplest form of anti-debug on Linux using `ptrace`.
func CheckPtrace() bool {
    if err := syscall.PtraceAttach(0); err != nil {
        return true
    }
    return false
}

// Main routine used in detecting debuggers for Linux, calling every known technique and
// returning a bool for later consumption
func LinuxDebugging() bool {

    // basic: ptrace check
    if CheckPtrace() == true {
        return true
    }

    return false
}
