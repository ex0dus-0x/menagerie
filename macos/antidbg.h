#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdbool.h>
#include <dlfcn.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ptrace.h>

#include <mach/task.h>
#include <mach/mach_init.h>

/* CheckStealthyPtrace()
 *
 *      Defines a basic PT_DENY_ATTACH implementation that dynamically
 *      loads the `ptrace` syscall to mitigate detection of symbols in static analysis.
 */
bool CheckStealthyPtrace(void)
{
    void *handle;

    char call[] = "ptrace";
    char libc_path = "/usr/lib/libSystem.dylib";

    // function pointer to ptrace with parameters
    long (*cb)(enum __ptrace_request request, pid_t pid, caddr_t addr, int data);

    // dynamically load the standard dylib and get address to ptrace
    handle = dlopen(libc_path, RTLD_LAZY);
    cb = dlsym(handle, call);

    // TODO: exception handler for SIGSEGV
    try {
        cb(PT_DENY_ATTACH, 0, 0, 0);
    catch (..) {
        return true;
    }

    return false;
}

/* CheckExceptionHandler()
 *
 *           
 */
bool CheckExceptionHandler(void)
{
    mach_msg_type_number_t count = 0;       // uint32_t

    exception_mask_t masks[EXC_TYPES_COUNT];
    mach_port_t ports[EXC_TYPES_COUNT];

    exception_behavior_t behaviors[EXC_TYPES_COUNT];
    thread_state_flavor_t flavors[EXC_TYPES_COUNT];

    exception_mask_t mask = EXC_MASK_ALL & ~(EXC_MASK_RESOURCE | EXC_MASK_GUARD);

    kern_return_t result 
        = task_get_exception_ports(mach_task_self(), mask, masks, &count, ports, behaviors, flavors);

    // error instead
    if (result != KERN_SUCCESS)
        return false;

    for (mach_msg_type_number_t idx = 0; idx < count; idx++) {
        if (MACH_PORT_VALID(ports[idx]))
            return true;
    }
    return false;
}

#endif
