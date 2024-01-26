#ifndef BVH_H
#define BVH_H


#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include <curand_kernel.h>


class bvh_node : public hittable {
public:

    __device__ bvh_node(hittable** list, int n, curandState* local_rand_state) {
        int axis = int(3 * curand_uniform(local_rand_state));
        
        bool swapped;
        //bubble sort
        
        for (int i = 0; i < n-1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (list[j]->bounding_box().axis(axis).min < list[j + 1]->bounding_box().axis(axis).min){
                    auto temp = list[j];
                    list[j] = list[j + 1] ;
                    list[j+1] = temp;
                    swapped = true;
                }
            }
            if (!swapped)
                break;
        }

        if (n == 1) {
            left = right = list[0];
        }
        else if (n == 2) {
            left = list[0];
            right = list[1];
        }
        else {
            left = new bvh_node(list, n / 2, local_rand_state);
            right = new bvh_node(list + n / 2, n - n / 2, local_rand_state);
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
    }
    __device__ bvh_node(hittable** src_objects, size_t start, size_t end, curandState* local_rand_state) {
        /*
        int axis = (int)(3*curand_uniform(local_rand_state));
        
        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;
            
        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        }
        else if (object_span == 2) {
            if (comparator(objects[start],objects[start+1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else {
            //todo sort better than bubble sort
            
            bool swapped;
            /*
            for (int i = start; i < end-1; i++) {
                swapped = false;
                for (int j = start; j < end - i - 1; j++) {
                    
                    if (1) {
                        auto temp = objects[j];
                        objects[j] = objects[j+1];
                        objects[j+1] = temp;
                        swapped = true;
                    }
                }
                if (!swapped)
                    break;
            }
            //qsort(src_objects, object_span, sizeof(hittable*), comparator);
            
            auto mid = start + object_span / 2;
            left = new bvh_node(objects, start, mid, local_rand_state);
            right = new bvh_node(objects, mid, end, local_rand_state); 
        }

        bbox = aabb(left->bounding_box(), right->bounding_box()); */
    }

    __device__ bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    __device__ aabb bounding_box() const override { return bbox; }

private:
    hittable* left;
    hittable* right;
    aabb bbox;
    __device__ int box_compare(const void* a, const void* b, int axis_index) {
        aabb box_left, box_right;
        hittable* ah = *(hittable**)a;
        hittable* bh = *(hittable**)b;
        
        if (ah->bounding_box().axis(axis_index).min < bh->bounding_box().axis(axis_index).min)
            return -1;
        else
            return 1;
    }

    __device__ int box_x_compare(const void* a, const void* b) {
        return box_compare(a, b, 0);
    }

    __device__ int box_y_compare(const void* a, const void* b) {
        return box_compare(a, b, 1);
    }

    __device__ int box_z_compare(const void* a, const void* b) {
        return box_compare(a, b, 2);
    }


};
#endif