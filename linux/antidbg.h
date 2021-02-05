#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <sys/ptrace.h>

bool CheckStealthyPtrace(void)
{
    void *handle;

    char call[] = "ptrace";
    char libc_path[] = "/usr/lib/libc.so";

    // function pointer to ptrace with parameters
    long (*cb)(enum __ptrace_request request, pid_t pid);

    // dynamically load libc shared object and get address to ptrace
    handle = dlopen(libc_path, RTLD_LAZY);
    cb = dlsym(handle, call);

    // execute to check if process has debugger parent
    if (cb(PTRACE_TRACEME, 0) == -1)
        return true;

    return false;
}


// TODO
bool CheckBreakpoint(void)
{
    //__asm__("int 3");
    return false;
}

#endif
