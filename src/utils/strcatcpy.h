#ifndef STRING_UTILS
#define STRING_UTILS

#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

size_t nstrcpy(char *dst, const char *src, size_t sz);
size_t nstrcat(char *dst, const char *src, size_t sz);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // STRING_UTILS
