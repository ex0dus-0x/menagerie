#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <signal.h>
#include <ctype.h>

#include <sys/ptrace.h>
#include <sys/stat.h>

/* side effect for signal handling */
static int isDebugged = -1;

/* handler for switching isDebugged if it first catches SIGTRAP */
static void handler(int _signum)
{
    isDebugged = 0;
    signal(SIGTRAP, SIG_DFL);
}

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

bool CheckBreakpoint(void)
{
    // isDebugged is reverted only if we catch rather than a debugger
    if (isDebugged == -1) {
        isDebugged = 1;
        signal(SIGTRAP, handler);
        raise(SIGTRAP);
    }
    return isDebugged;
}


bool CheckProcessFingerprint(void)
{
    // open procfs path to parse metadata about process
    char procpath[] = "/proc/self/status";
    int fd = open(procpath, O_RDONLY);
    if (fd == -1)
        return false;

    // read file into buffer
    char buffer[4096];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes <= 0)
        return false;
    buffer[bytes] = '\0';

    // check if tracer key is in configuration
    char tracer[] = "TracerPid:";
    char *bufptr = strstr(buffer, tracer);
    if (!bufptr)
        return false;

    // check to see if value set is = 0
    for (char *ptr = bufptr + sizeof(tracer) - 1; ptr <= buffer + bytes; ++ptr) 
    {
        if (isspace(*ptr))
            continue;
        else if ((isdigit(*ptr) != 0) && ((*ptr) != '0'))
            return true;
    }

    return false;
}


bool CheckProcessHeapRelocate(void)
{
    static unsigned char bss;
    unsigned char *probe = malloc(0x10);

    // check if heap allocated region is in higher memory address range
    // that denotes that it has been manually relocated.
    if (probe - &bss > 0x20000)
        return true;

    return false;
}

#endif
