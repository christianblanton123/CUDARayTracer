#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>
#include <limits>
#include <cuda_runtime.h>

#ifndef infinity
#define infinity ((float)((float)(1e+300 * 1e+300)))
#endif
class interval {
public:

	float min, max;
	__host__ __device__ interval(float min, float max) :min(min), max(max) {};
	__host__ __device__ interval() : min(-infinity), max(-infinity) {};
	__host__ __device__ interval(const interval& a, const interval& b) :min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {};
	__host__ __device__ float size() { return max - min; };
	__host__ __device__ bool contains(float b) { return min <= b && b <= max; };
	__host__ __device__ bool surrounds(float b) { return min < b && b < max; };
	__host__ __device__ float clamp(float x) const { 
		if (x < min)return min;
		if( x > max) return max;
		return x;
	}
	__host__ __device__ interval expand(float delta) const{
		float padding = delta / 2;
		return interval(min - padding, max + padding);

	}
	static const interval empty;
	static const interval universe;
};

const interval interval::empty=interval(-infinity, -infinity);
const interval interval::universe=interval(- infinity, infinity);

__host__ __device__ interval operator+(const interval& i, float displacement) {
	return interval(i.min + displacement, i.max + displacement);
}
__host__ __device__ interval operator+(double displacement, const interval& i) {
	return i + displacement;
}


#endif // !INTERVAL_H

