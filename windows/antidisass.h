#ifndef ANTIDISASS_H
#define ANTIDISASS_H

inline void AntiDisassembly(void)
{
    __asm__(".byte 0xeb,0xff,0xc0");
}

#endif
