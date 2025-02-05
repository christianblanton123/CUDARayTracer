#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hittable.h"

class material {
public:
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, curandState* local_rand_state) const = 0;
};

class lambertian : public material {
public:
    __device__ lambertian(const vec3& a) : albedo(a) {}
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, curandState* local_rand_state) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
        if (target.near_zero()) {
            target = rec.normal;
        }
        scattered = ray(rec.p, target - rec.p, r_in.time());
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

class metal : public material {
public:
    __device__ metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, curandState* local_rand_state) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(local_rand_state), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0.0f);
    }
    vec3 albedo;
    float fuzz;
};

class dielectric : public material {
public:
    __device__ dielectric(float index_of_refraction) : ir(index_of_refraction) {}
    __device__ virtual bool scatter(const ray& r_in,
        const hit_record& rec,
        vec3& attenuation,
        ray& scattered,
        curandState* local_rand_state) const {
        attenuation = vec3(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
        bool cannot_refract;
        if (refraction_ratio * sin_theta > 1.0f) //double floating point instructions happen if I don't do it like this
            cannot_refract = true;
        vec3 direction;
        
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > curand_uniform(local_rand_state))
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction,r_in.time());
        return true;
    }
    private:
    __device__ float reflectance(float cosine, float ref_idx) const{
            float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
            r0 = r0 * r0;
            return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
    }
    float ir;
};
#endif