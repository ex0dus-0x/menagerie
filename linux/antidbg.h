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

/* requires linking, re-implement to get rid of */
#ifdef ANTI_TELEMETRY
#include <systemd/sd-journal.h>
#endif

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
    char *real = realpath(".", "");
    return false;
}

#ifdef ANTI_TELEMETRY
bool CheckEBPFTracer(void)
{
    /* open new journal object */
    sd_journal *j;
    if (sd_journal_open(&j, SD_JOURNAL_SYSTEM) < 0)
        return -1;

    /* get file descriptor and prepare for consumption */
    if (sd_journal_get_fd(j) < 0)
        return -1;

    /* seek to tail of event logs */
    if (sd_journal_seek_tail(j) < 0)
        return -1;

    /* add filter on only audit events */
    sd_journal_add_match(j, "_TRANSPORT=audit", 0);
    sd_journal_add_conjunction(j);

    /* call first to be able to get individual events */
    if (sd_journal_previous(j) < 0)
        return -1;

    /* we'll only traverse back to these number of entries. If there is a BPF program load in this
     * set, we can assume that a security tracer became active as we ran our malware */
    int count = 5;

    /* flags gets set if a BPF program load is detected in the previous `count` entries */
    int bpf_load = 0;
    int bpf_unload = 0;

    /* TODO: for more granularity, we can also check and store the BPF program ID */

    /* use convenient macro to iterate through most recent entries */
    SD_JOURNAL_FOREACH_BACKWARDS(j) {

        if (count == 0)
            break;

        const char *data;
        size_t len;

        /* get message and parse if BPF call is involved */
        sd_journal_get_data(j, "MESSAGE", (const void **)&data, &len);
        if (strstr(data, "MESSAGE=BPF") != NULL) {

            /* check if the program was loaded or unloaded */
            if (strstr(data, "op=LOAD") != NULL)
                bpf_load = 1;
            else if (strstr(data, "op=UNLOAD") != NULL)
                bpf_unload = 1;
        }
        count--;
    }

    /* cleanup */
    sd_journal_close(j);

    /* if the previous entries contained a full load cycle, we can assume that the program was
     * detached and not monitoring us. However, if there is a load and no unload, we can assume it's
     * still watching us.
     */
    if (bpf_load && !bpf_unload) {
        return true;
    }
    return false;
}
#endif

#endif
