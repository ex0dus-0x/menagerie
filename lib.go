package ems

// Runs cross-platform anti-debug detection, and run a callback routine if debugging is detected.
func AntiDebuggingCb(cb func()) {
    if AntiDebugging() == true {
        cb()
    }
}

// Runs cross-platform anti-sandbox detection, and run a callback routine if debugging is detected.
func AntiSandboxCb(cb func()) {
    if AntiSandbox() == true {
        cb()
    }
}
