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


}

#endif
