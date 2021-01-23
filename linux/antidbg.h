#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <sys/ptrace.h>

/* Defines a basic PTRACE_TRACEME implementation that dynamically loads the `ptrace` syscall from
 * libc to mitigate the detection of symbols in static analysis 
 */
bool CheckPtrace(void) {
    void *handle;
    char call[] = "ptrace";

    // function pointer to ptrace with parameters
    long (*cb)(enum __ptrace_request request, pid_t pid);

    // dynamically load libc shared object and get address to ptrace
    handle = dlopen("/usr/lib/libc.so", RTLD_LAZY);
    cb = dlsym(handle, call);

    // TODO: do ptrace call
    if (cb(PTRACE_TRACEME, 0) == -1) {
        return true;
    }
    return false;
}


#endif
