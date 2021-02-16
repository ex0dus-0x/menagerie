#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* const flag set when signal handling */
static int isDebugged = 1;

/* handler for switching isDebugged if it first catches SIGTRAP */
static void handler(int _signum)
{
    isDebugged = 0;
    signal(SIGTRAP, SIG_DFL);
}

bool CheckBasicPtrace(void)
{
    if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1)
        return true;
    return false;
}

bool CheckBreakpoint(void)
{
    // isDebugged is reverted only if we catch SIGTRAP rather than a debugger
    signal(SIGTRAP, handler);
    raise(SIGTRAP);
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

    // check to see if value set is 0 for no debugger
    for (char *ptr = bufptr + sizeof(tracer) - 1; ptr <= buffer + bytes; ++ptr) 
    {
        if (isspace(*ptr))
            continue;
        else
            return isdigit(*ptr) != 0 && *ptr != '0';
    }
    return false;
}

bool CheckProcessHeapRelocate(void)
{
    static unsigned char bss;
    unsigned char *probe = malloc(0x10);

    // check if heap allocated region is in higher memory address range
    // that denotes that it hasn't been manually relocated.
    if (probe - &bss > 0x20000)
        return false;

    return true;
}

/* TODO: check readlink */
bool CheckCallerName(void)
{
    return false;
}

#endif
