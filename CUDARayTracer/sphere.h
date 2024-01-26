#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

class sphere : public hittable {
public:
    __device__ sphere() {}
    __device__ sphere(vec3 cen, float r, material* m) : center1(cen), radius(r), mat(m), is_moving(false) 
    {
        vec3 rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    };
    __device__ sphere(vec3 cen1, vec3 cen2, float r, material* m) : center1(cen1), radius(r), mat(m), is_moving(true) 
    {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(cen1 - rvec, cen1 + rvec);
        aabb box2(cen2 - rvec, cen2 + rvec);
        bbox = aabb(box1, box2);
        center_vec = cen2 - cen1;
    };
    __device__ virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const;
    __device__ material* get_mat() { return mat; }
    __device__ aabb bounding_box() const override { return bbox; }
private:
    bool is_moving;
    vec3 center1;
    vec3 center_vec;
    float radius;
    material* mat;
    aabb bbox;
    __device__ vec3 sphere_center(float time) const {
        return center1 + time * center_vec;
    }
};

__device__ bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 center = is_moving ? sphere_center(r.time()) : center1;
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    float sqrtd = sqrt(discriminant);
    if (discriminant > 0) {
        float root  = (-b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-b + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
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