// Smoke test sample to compile for each platform demonstrating all anti-analysis capabilities.
package main

import (
    "fmt"
    "github.com/ex0dus-0x/menagerie"
)

func sensitive() {}

func init() {
    fmt.Println("Performing anti-debug checks...")
    if res := menagerie.CheckDebuggerBasic(); res != false {
        fmt.Println("CheckDebuggerBasic - Caught!")
    }

    if res := menagerie.ThrowBreakpointExcept(); res != false {
        fmt.Println("ThrowBreakpointExcept - Caught!")
    }

    if res := menagerie.BreakpointChecksumAt(sensitive); res != false {
        fmt.Println("BreakpointChecksumAt - Caught!")
    }
}

func main() {
    fmt.Println("Starting the program...")
}
