#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)

#include <winternl.h>
#include <excpt.h>
#include <processthreadsapi.h>

#elif defined(__unix__)

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/wait.h>

#elif defined(__APPLE__)

#include <sys/types.h>
#include <sys/ptrace.h>

#include <mach/task.h>
#include <mach/mach_init.h>

#endif

#include "antidbg.h"

// global flag to switch when exception handler is triggered
#ifdef __unix__
static bool isDebugged = true;
#elif defined(__APPLE__)
static bool isDebugged = false;
#endif

/* Callback for exception handling a variety of debugger detections */
#if defined(__unix__) || defined(__APPLE__)
static void except_handler(int sig)
{
#ifdef __unix__
    isDebugged = false;
    //signal(SIGTRAP, SIG_DFL);
#elif defined(__APPLE__)
    isDebugged = true;
#endif
}
#elif defined(_WIN32) || defined(_WIN64)
static long CALLBACK
except_handler(EXCEPTION_POINTERS *pointers)
{
    switch(pointers->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_BREAKPOINT:
        return EXCEPTION_EXECUTE_HANDLER;
    default:
        return EXCEPTION_CONTINUE_SEARCH;
    }
}
#endif


/* Runs basic most rudimentary debugger check for every operating system */
bool CheckDebuggerBasic(void)
{
    // Enumerate debug flags on the PEB
#if defined(_WIN32) || defined(_WIN64)

    // TODO: big-brain, check if PEB structure itself was patched before hand as a sanity

    // parse out the PEB from FS or GS offsets
    PPEB pPeb;

#ifdef _WIN32
    __asm__("movl %%fs:0x30, %0" : "=r" (pPeb) : : );
#else
    __asm__("movl %%gs:0x60, %0" : "=r" (pPeb) : : );
#endif // _WIN64

    // Do checks one-by-one for complicated control-flow
    // to figure out by reverse engineer

    // check for basic BeingDebugged
    if (pPeb->BeingDebugged)
        return true;

#ifdef _WIN32
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0x68);
#else
    DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE) pPeb + 0xbc);
#endif

    // check NtGlobalFlag to be set as 0x70:
    // TODO: annotate the flags that are included to make up 0x70
    if (dwNtGlobalFlag & 0x70)
        return true;

    // parse out pointer to heap from PEB, and two attributes' offsets for antidebug
#ifdef _WIN32
    PVOID pHeap = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x18));
    DWORD Flags = *(PDWORD)((PBYTE) pHeap + 0x40);
    DWORD ForceFlags = *(PDWORD)((PBYTE) pHeap + 0x44);
#else
    PVOID pHeap = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x30));
    DWORD Flags = *(PDWORD)((PBYTE pHeap + 0x70);
    DWORD ForceFlags = *(PDWORD)((PBYTE pHeap + 0x74);
#endif

    if (Flags & ~HEAP_GROWABLE || ForceFlags)
        return true;
    return false;
    
#elif defined(__unix__)

    if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1)
        return true;
    return false;

#elif defined(__APPLE__)

    signal(SIGINT, handler);
    ptrace(PT_DENY_ATTACH, 0, 0, 0);
    return isDebugged;

#endif
}

bool ThrowBreakpointExcept(void) 
{
#if defined(_WIN32) || defined(_WIN64)
    __try1 (except_handler) {
        __asm__("int3");
    } __except1 {
        return false;
    }
    return true;

#elif defined(__unix__) || defined(__APPLE__)

    signal(SIGTRAP, except_handler);
    __asm__("int3");

#if defined(__unix__)
    return isDebugged;
#elif defined(__APPLE__)
    return !isDebugged;
#endif
#endif
}

/*
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
*/

/* Iterate over disassembly of a function and determine if breakpoint instrumentation is present */
bool BreakpointChecksumAt(void* function)
{
    unsigned char* pBytes = (unsigned char*) function;
    for (size_t i = 0; ; i++) {

        // finalize at ret instruction
        if (pBytes[i] == 0xc3)
            break;

        // return true when encountering breakpoint instrumentation
#if defined(_WIN32) || defined(_WIN64)
        if (pBytes[i] == 0xcc)
#else
        if (pBytes[i] & 0xff == 0xcc)
#endif
            return true;
    }
    return false;
}

/* Iterate over context debug registers to check if any are set, signifying
 * hardware breakpoints used to halt execution.
 */
#if defined(_WIN32) || defined(_WIN64)
bool CheckHardwareBreakpoints(void)
{
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(GetCurrentThread(), &ctx))
        return false;
    return ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3;
}
#endif


/* Introspects current memory-mapping to see if any image relocations were manually done by a debugger */
bool CheckMemoryFingerprint(void)
{
    static unsigned char bss;
    unsigned char *probe = malloc(0x10);

    // check if heap allocated region is in higher memory address range
    // that denotes that it hasn't been manually relocated.
    if (probe - &bss > 0x20000)
        return false;

    return true;
}

/* Use operaing system facilities to query whether or not a parent tracer process exists */
bool CheckParentTracer(void)
{
#if defined(__unix__)
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
    // TODO: self implementation of strstr
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
#endif

    return false;
}
