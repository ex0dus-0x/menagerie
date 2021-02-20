#ifndef ANTIVM_H
#define ANTIVM_H

#include <stdbool.h>

// TODO
bool CheckInstTiming(void)
{
    unsigned int bench1 = 0, bench2 = 0;

    if ((bench2 - bench1) > 100)
        return true;

    return false;
}

#endif
