#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
};

bool sphere::hit(const ray&r, double t_min, double t_max, hit_record& rec) const {
    // b*b*t^2 + 2 * b * (A - C) * t + (A - C)^2 - r^2
    // ax^2 + bx + c
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto dis = half_b*half_b - a*c;
    if (dis < 0) {
        return false;
    } 
    
    // Find the nearest root that lies in the range;
    auto sqrtdis = sqrt(dis);
    auto root = (-half_b - sqrtdis) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtdis) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(root);
    rec.normal = (rec.p - center) / radius;
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif