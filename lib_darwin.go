// Implements main functionality for anti-reversing routines. Will run detection
// routine based on specific platform the adversarial sample is on.
package ems

import "github.com/ex0dus-0x/ems/macos"

/*=========================== ANTI-DEBUGGING ===========================*/

// Cross-platform function used to detect if a debugger is currently hooked onto the
// current executable.
func AntiDebugging() bool {
    // basic: PT_DENY_ALLOW antidebug check
    if macos.CheckPtrace() == true {
        return true
    }
    return false
}

// Runs cross-platform anti-debug detection, and run a callback routine if debugging is detected.
func AntiDebuggingCb(cb func()) {
    if AntiDebugging() == true {
        cb()
    }
}

/*=========================== ANTI-SANDBOXING ===========================*/

func AntiSandbox() bool {
    // basic: check `hw.model`
    if macos.CheckHardwareFingerprint() {
        return true
    }
    return false
}

// Runs cross-platform anti-sandbox detection, and run a callback routine if debugging is detected.
func AntiSandboxCb(cb func()) {
    if AntiSandbox() == true {
        cb()
    }
}

/*=========================== ANTI-DISASSEMBLY ===========================*/

// Cross-platform function used to inject confusing assembler payloads before sensitive
// functionality to trick any linear or recursive disassembler.
func AntiDisassembly() {
}
