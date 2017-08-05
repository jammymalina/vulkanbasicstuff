#ifndef LINSPACE_H 
#define LINSPACE_H

#include <stddef.h>
#include <stdint.h>

static inline void linspace(float *result, float a, float b, uint32_t n) {
	if (n < 2) {
		if (n == 1) {
			result[0] = a;
		}
		return;
	}
	n--; 
	for (size_t i = n; i >= 0; i--) {
		result[i] = (i * b + (n - i) * a) / n; 
	}	
}

#endif 

