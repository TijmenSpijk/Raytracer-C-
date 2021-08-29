#include "./classes/color.h"
#include "./classes/vec3.h"
#include "./classes/ray.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r) {
    // b*b*t^2 + 2 * b * (A - C) * t + (A - C)^2 - r^2
    // ax^2 + bx + c
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto dis = b*b - 4*a*c;
    return (dis>0);
}

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_heigth = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_heigth = 2.0;
    auto viewport_width = aspect_ratio * viewport_heigth;
    auto focal_length = 1.0;
    
    auto origin = point3(0,0,0);
    auto horizotal = vec3(viewport_width,0,0);
    auto vertical = vec3(0,viewport_heigth,0);
    auto lower_left_corner = origin - horizotal/2 - vertical/2 - vec3(0,0,focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_heigth << "\n255\n";

    for (int j = image_heigth-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_heigth-1);
            ray r(origin, lower_left_corner + u*horizotal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}