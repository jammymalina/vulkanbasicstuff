#ifndef STRING_UTILS
#define STRING_UTILS

#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

char*  nstrncpy(char *dst, const char *src, size_t n);
size_t nstrlcpy(char *dst, const char *src, size_t sz);
size_t nstrcat(char *dst, const char *src, size_t sz);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // STRING_UTILS
