#ifndef AABB_H
#define AABB_H
#include "interval.h"
#include "ray.h"
#include "vec3.h"
class aabb {
	public:
	interval x, y, z;
	__device__ aabb() {}
	__device__ aabb(interval a, interval b, interval c) : x(a), y(b), z(c) {};
	__device__ aabb(vec3 a, vec3 b){
		x = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
		y = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
		z = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
	}
	__device__ aabb(aabb a,aabb b) 
	{
		x = interval(a.x, b.x);
		y = interval(a.y, b.y);
		z = interval(a.z, b.z);
	};
	__device__ const interval& axis(int n) const {
		if (n == 2)
			return z;
		if (n == 1)
			return y;
		return x;
	}

	__device__ bool hit(const ray& r, interval ray_t) const {
		for (int a = 0; a < 3; a++) {
			float invD = 1 / r.direction()[a];
			float orig = r.origin()[a];

			auto t0 = (axis(a).min - orig) * invD;
			auto t1 = (axis(a).max - orig) * invD;

			if (invD < 0) {
				auto temp = t0;
				t0 = t1;
				t1 = temp;
			}
		
			if (t0 > ray_t.min) ray_t.min = t0;
			if (t1 < ray_t.max) ray_t.max = t1;

			if (ray_t.max <= ray_t.min)
				return false;
		}
		return true;
	}


};

#endif