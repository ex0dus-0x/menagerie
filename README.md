# The Anti-Analysis Menagerie

Cross-platform malware development library for anti-analysis techniques.

## Design Goals

* Provide a rich and convenient interface for defensive evasion for Golang, a popular choice for red teams and malware engineers.
* Incorporate "defensive software development" (ie. minimizing `call`s, confusing control flow) to make reverse engineering even more difficult
* Use as a learning resource for both attack and mitigation, and a collaborative project to contribute new techniques

## Techniques Supported

### Anti-Debugging

| Technique              | Linux         | Windows                   | macOS    |
|------------------------|---------------|---------------------------|----------|
| Basic                  | `ptrace`      | Process Environment Block | `ptrace` |
| Breakpoint             | SIGTRAP       | WIP                       | WIP      |
| Process Fingerprinting | Procfs        | WIP                       | WIP      |
| Memory Fingerprinting  | Heap Relocate | WIP                       | WIP      |

### Anti-Virtualization

|               | Linux   | Windows | macOS   |
|---------------|---------|---------|---------|
| CPU Profiling | `cpuid` | `cpuid` | sysctl  |


### Anti-Disassembly

|                        | Linux | Windows | macOS |
|------------------------|-------|---------|-------|
| Impossible Disassembly | ❌     | ✔️      | ❌    |


### Anti-Telemetry

| Technology             | Platform      | Technique           |
|------------------------|---------------|---------------------|
| eBPF                   | Linux         | journald BPF check  | 

### Contributing

Have another technique you want curated? Create a pull request!

Note that Windows detection techniques should all be MinGW-compliant!
