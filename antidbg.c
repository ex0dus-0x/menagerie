#if defined(_WIN32) || defined(_WIN64)

#include <winternl.h>
#include <excpt.h>
#include <processthreadsapi.h>

#elif defined(__UNIX__)

#include <stdlib.h>
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
#ifdef __UNIX__
static bool isDebugged = true;
#elif defined(__APPLE__)
static bool isDebugged = false;
#endif

#if defined(__UNIX__) || defined(__APPLE__)
static void except_handler(int sig)
{
#ifdef __UNIX__
    isDebugged = false;
    signal(SIGTRAP, SIG_DFL);
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


bool CheckDebuggerBasic(void)
{
#if defined(_WIN32) || defined(_WIN64)

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
    
#elif defined(__UNIX__)

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

#elif defined(__UNIX__) || defined(__APPLE__)

    signal(SIGTRAP, except_handler);
    raise(SIGTRAP);
    return isDebugged;

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

bool BreakpointChecksumAt(void)
{
    return false;
}

#if defined(_WIN32) || defined(_WIN64)
// Iterate over context debug registers to check if any are set, signifying
// hardware breakpoints used to halt execution.
bool CheckHardwareBreakpoints(void)
{
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(GetCurrentThread(), &ctx))
        return false;
    return ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3;
}
#endif


bool CheckMemoryFingerprint(void)
{
    return false;
}

bool CheckParentTracer(void)
{
    return false;
}
