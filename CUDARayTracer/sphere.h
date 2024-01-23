#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

class sphere : public hittable {
public:
    __device__ sphere() {}
    __device__ sphere(vec3 cen, float r, material* m) : center1(cen), radius(r), mat(m), is_moving(false) {};
    __device__ sphere(vec3 cen1, vec3 cen2, float r, material* m) : center1(cen1), radius(r), mat(m), is_moving(true) 
    {
        center_vec = cen2 - cen1;
    };
    __device__ virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    __device__ material* get_mat() { return mat; }
private:
    bool is_moving;
    vec3 center1;
    vec3 center_vec;
    float radius;
    material* mat;

    __device__ vec3 sphere_center(float time) const {
        return center1 + time * center_vec;
    }
};

__device__ bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 center = is_moving ? sphere_center(r.time()) : center1;
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    float sqrtd = sqrt(discriminant);
    if (discriminant > 0) {
        float root  = (-b - sqrtd) / a;
        if (!(root < t_max && root > t_min)) {
            root = (-b + sqrtd) / a;
            if (!(root < t_max && root > t_min)) {
                return false;
            }

        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.mat = mat;
        rec.set_face_normal(r, outward_normal);
        return true;
       
    }
    return false;
}


#endif