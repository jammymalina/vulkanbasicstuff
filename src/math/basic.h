#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define min(x, y) ((x) < (y) ? (x)  : (y))
#define max(x, y) ((x) > (y) ? (x)  : (y))
#define abs(a)	  (((a) < 0) ? -(a) : (a))
#define clamp(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define degrees_to_radians(angle_degrees) (angle_degrees * M_PI / 180.0)
#define radians_to_degrees(angle_radians) (angle_radians * 180.0 / M_PI)

#endif // BASIC_MATH_H 

