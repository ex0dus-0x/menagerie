inline void payload(void) 
{
    __asm__(".byte 0xeb,0xff,0xc0");
}
