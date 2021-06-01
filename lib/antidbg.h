#ifndef ANTIDBG_H
#define ANTIDBG_H

#include <stdbool.h>

bool CheckDebuggerBasic(void);

/* Breakpoint checks */
bool ThrowBreakpointExcept(void);
bool BreakpointChecksumAt(void* pMemory);

#if defined(_WIN32) || defined(_WIN64)
bool CheckHardwareBreakpoints(void);
#endif

/* Enumerate memory mappings */
bool CheckMemoryFingerprint(void);

/* Parent Process exists */
bool CheckParentTracer(void);

#endif
