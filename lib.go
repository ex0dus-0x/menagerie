// Implements main functionality for anti-reversing routines. Will run detection
// routine based on specific platform the adversarial sample is on.
package ems

import (
    "runtime"
    "github.com/ex0dus-0x/ems/windows"
    "github.com/ex0dus-0x/ems/linux"
    //"github.com/ex0dus-0x/ems/macos"
)

/*=========================== ANTI-DEBUGGING ===========================*/

// Cross-platform function used to detect if a debugger is currently hooked onto the
// current executable.
func AntiDebugging() bool {
    if runtime.GOOS == "windows" {
        return windows.WindowsDebugging()
    } else if runtime.GOOS == "linux" {
        return linux.LinuxDebugging()
    }
    /*
    else if runtime.GOOS == "macos" {
        return macos.MacDebugging()
    }
    */

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
    /*
    if runtime.GOOS == "windows" {
        return WindowsSandbox()
    } else if runtime.GOOS == "linux" {
        return LinuxSandbox()
    } else if runtime.GOOS == "macos" {
        return MacSandbox()
    }
    */
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
