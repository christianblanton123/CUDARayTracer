#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
    __device__ ray() {}
    __device__ ray(const vec3& a, const vec3 b) :A(a), B(b), tm(0){}
    __device__ ray(const vec3& a, const vec3 b, float time) : A(a), B(b), tm(time) {}
    __device__ vec3 origin() const { return A; }
    __device__ float time() const { return tm; }
    __device__ vec3 direction() const { return B; }
    __device__ vec3 at(float t) const { return A + t * B; }
private:
    vec3 A;
    vec3 B;
    float tm;
};

#endif