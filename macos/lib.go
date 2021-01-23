package linux

import (
    "syscall"
    "os/signal"
)

// Simplest form of anti-debug on macOS, attempting to trigger and catch a SEGV when attempting
// to `ptrace` with PT_DENY_ATTACH
func CheckPtrace() bool {
    // stores final state that gets changed
    debugged := false

    sigchan := make(chan os.Signal, 1)
    exitchan := make(chan bool, 1)

    signal.Notify(sigchan, syscall.SIGSEGV)
    go func() {
        sig := <-sigchan
        debugged = true
        exitchan <- true
    }()

    // check for debugger - first call may be hooked
    syscall.RawSyscall(syscall.SYS_PTRACE, uintptr(syscall.PT_DENY_ATTACH), 0, 0)

    <-exitchan
    return debugged
}
