package menagerie

// #cgo CFLAGS: -Ilib
// #include "antidbg.h"
// #include "antivm.h"
import "C"


func CheckHardwareBreakpoints() bool {
    return bool(C.CheckHardwareBreakpoints())
}
