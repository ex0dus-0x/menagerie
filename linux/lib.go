package linux

import "syscall"

// Simplest form of anti-debug on Linux using `ptrace`.
func CheckPtrace() bool {
    // check for debugger
    _, _, res := syscall.RawSyscall(syscall.SYS_PTRACE, uintptr(syscall.PTRACE_TRACEME), 0, 0)
    if res == 1 {
        return true
    }
    return false
}
