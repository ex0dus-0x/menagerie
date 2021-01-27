package macos

import (
    "syscall"
    "strings"
)

func CheckHardwareFingerprint() -> bool {
    res, err := syscall.Sysctl("hw.model")
    if err != nil {
        return false
    }

    if !strings.Contains(res, "Mac") {
        return true
    }
    return false
}
