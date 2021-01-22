package macos

import "syscall"

// Simplest form of anti-debug on macOS using `ptrace` with `PT_DENY_ATTACH`, called twice to
// ensure that the detection works if any hooking is done on the `ptrace` call itself
func CheckPtrace() bool {
    state := 0

    // check for debugger - first called may be hooked
    if _, _, res := syscall.RawSyscall(syscall.SYS_PTRACE, uintptr(syscall.PT_DENY_ATTACH), 0, 0); res == 1 {
        state = 1

        // check for debugger again - second call to double-check
        if _, _, res := syscall.RawSyscall(syscall.SYS_PTRACE, uintptr(syscall.PT_DENY_ATTACH), 0, 0); res == 1 {
            state = 2
        }
    }
    return state == 2
}
