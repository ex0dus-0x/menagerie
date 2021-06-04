# The Anti-Analysis Menagerie

Cross-platform malware development library for anti-analysis techniques.

## Design Goals

* Provide a rich and convenient interface for defensive evasion for Golang, a popular choice for red teams and malware engineers.
* Use as a learning resource for both attack and mitigation, and a collaborative project to contribute new techniques

## Techniques Supported

### Debugger Detection:

```go
// Most standard debugger checks for each platform
func CheckDebuggerBasic()

// Breakpoints: exception and checksums
func CheckThrowBreakpoint()
func BreakpointChecksumAt(ptr interface{})
func CheckHardwareBreakpoints() // Win only!

// Process Mappings Check
func CheckMemoryFingerprint()

// Parent Process Fingerprinting
func CheckParentTracer()
```

### Virtual Machine Detection:

```go
// Profile CPUID for VM features
func CheckCPUIDIsVM()
func CheckCPUIDHypervisor()

// VM Process Enumeration (WIP)
func CheckVMProcesses()
```

### Telemetry Monitoring Detection:

```go

// Linux Only - requires dynamically linking journald
func CheckEbpfTracer()
```

## Contributing

Have another technique you want curated? Create a pull request!
