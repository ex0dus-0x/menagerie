package linux

import (
    "syscall"
    "os/signal"
)

// Simplest form of anti-debug on Linux using `ptrace`, called twice to ensure that the detection
// works if any hooking is done on the `ptrace` call itself.
func CheckPtrace() bool {
    // stores final state that gets changed
    debugged := false

    sigchan := make(chan os.Signal, 1)
    signal.Notify(sigchan, syscall.SIGSEGV)


    exitchan := make(chan int)
    go func() {

    }

    // check for debugger - first call may be hooked
    syscall.RawSyscall(syscall.SYS_PTRACE, uintptr(syscall.PT_DENY_ATTACH), 0, 0)
    return debugged
}
