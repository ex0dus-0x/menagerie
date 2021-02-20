/*
 * deps.h
 *
 *      Re-implementations of common C/C++ operations for use across all OS platforms.
 *      This is used to prevent the need to import from libc or any dependency that 
 *      requires dynamic loading, something an analyst can easily patch out.
 */


/* Re-implementation of BSD-style strncmp */
static inline int
inline_strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    do {
        if (*s1 != *s2++)
            return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));

        if (*s1++ == 0)
            break;

    } while (--n != 0);
    return 0;
}
