#ifndef ANTIVM_H
#define ANTIVM_H

#include <stdbool.h>

#include <sys/types.h>
#include <sys/sysctl.h>

bool CheckSysctl(void)
{
    int mib[2];
    char buf[20];

    mib[0] = CTL_HW;
    mib[1] = HW_MODEL;

    sysctl(mib, 2, buf, 20, NULL, 0 );
    return false;
}

#endif
