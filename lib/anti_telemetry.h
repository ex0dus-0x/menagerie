#ifndef ANTI_TELEMTRY_H
#define ANTI_TELEMTRY_H

#include <stdbool.h>

#ifdef __UNIX__
bool CheckEBPFTracer(void);
#endif

#endif
