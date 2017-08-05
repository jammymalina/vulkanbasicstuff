#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <stddef.h>
#include <stdint.h>

typedef float (*float_func)(float); 

static inline float FLOAT_ONE_FUNC(float x) {
	return 1.0;
}

static inline void uint_range_from(uint32_t *result, uint32_t from, uint32_t n) {
	for (size_t i = 0; i < n; i++) {
		result[i] = from + i;
	}
}

static inline void float_range_from(float *result, float from, uint32_t n) {
	for (size_t i = 0; i < n; i++) {
		result[i] = from + i;
	}
}

static inline void float_constant_range(float *result, float c, uint32_t n) {
	for (size_t i = 0; i < n; i++) {
		result[i] = c;
	}
}

static inline void float_range(float *y, float *x, float_func func, uint32_t n) {
   for (size_t i = 0; i < n; i++) {
	   y[i] = func(x[i]); 
   }	   
}

#endif // MATH_FUNCTION_H

