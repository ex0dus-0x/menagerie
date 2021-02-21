// Smoke test sample to compile for each platform demonstrating all anti-analysis capabilities.
package main

import (
    "os"
    "fmt"
    "github.com/ex0dus-0x/menagerie"
)

func init() {
    fmt.Println("Performing anti-debug checks...")
    if menagerie.AntiDebugging() == true {
        fmt.Println("Debugger detected!")
        os.Exit(-1)
    }

    if menagerie.AntiVM() == true {
        fmt.Println("Hypervisor/sandbox detected!")
        os.Exit(-1)
    }
}

func main() {
    menagerie.AntiDisassembly()
    fmt.Println("Starting the program...")
}
