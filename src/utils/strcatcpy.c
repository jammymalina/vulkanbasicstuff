#include "strcatcpy.h"

size_t strlcpy(char* dst, const char* src, size_t sz) {
	char *d = dst;
	const char *s = src;
	size_t n = sz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (sz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++);
	}

	return(s - src - 1);	/* count does not include NUL */
}

size_t strlcat(char *dst, const char *src, size_t sz) {
	char *d = dst;
	const char *s = src;
	size_t n = sz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = sz - dlen;

	if (n == 0)
		return dlen + strlen(s);

	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return dlen + (s - src);
}
