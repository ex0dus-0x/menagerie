package linux

// #cgo CFLAGS: -g -Wall
// #include "linux.h"
import "C"

func LinuxDebugging() bool {
    return false
}
