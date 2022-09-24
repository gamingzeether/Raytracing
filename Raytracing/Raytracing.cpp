#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

#include "aabb.h"
#include "camera.h"
#include "frame.h"
#include "lightsource.h"
#include "material.h"
#include "plane.h"
#include "ray.h"
#include "sphere.h"
#include "shape.h"
#include "vec3.h"
#include "postprocessing.h"

// Defines
 #define DISPLAY_PROGRESS
// #define HIGH_QUALITY
// #define LOW_QUALITY
 #define MULTITHREADED

#ifdef HIGH_QUALITY
#undef LOW_QUALITY
#endif

// Define constants
#define PI 3.1415926535897932385
#define RAD_TO_DEG (PI / 180)
#define DEG_TO_RAD (180 / PI)

// Constants from defines
#ifdef MULTITHREADED
const int thread_count = 8;
#endif

#ifdef LOW_QUALITY
const int image_width = 256;
const int image_height = 256;
const int max_sample_rays = 10;
#else
#ifdef HIGH_QUALITY
const int image_width = 1024;
const int image_height = 1024;
const int max_sample_rays = 30;
#else
const int image_width = 512;
const int image_height = 512;
const int max_sample_rays = 20;
#endif
#endif
const int max_recur = 3;
const int color_max = 255;
const int cam_rays_per_pixel = 2;

camera cam = camera(image_width / image_height, image_height, 20, 10);
std::vector<lightsource*> lights;
std::vector<shape*> all_shapes;

inline double rand01() {
    return double(rand()) / RAND_MAX;
}

color ray_color(const ray& light_ray, int recur_depth, int ignore) {
    vec3 ray_direction = light_ray.direction().normalized();

    // Check if ray intersects with any shapes
    shape::intersect_info closest_intersect = shape::intersect_info();
    for (int i = 0; i < all_shapes.size(); i++) {
        if (i == ignore) {
            continue;
        }
        const shape* s = all_shapes[i];

        shape::intersect_info intersect = s->ray_intersects(light_ray);
        if (intersect.distance >= 0) {
            if (closest_intersect.distance < 0 || intersect.distance < closest_intersect.distance) {
                closest_intersect = intersect;
            }
        }
    }

    // Calculate color from intersection
    color c;
    if (closest_intersect.distance >= 0) {
        shape* intersect_shape = all_shapes[closest_intersect.shape_id];
        float roughness = intersect_shape->shape_material().material_roughness();
        c = intersect_shape->shape_material().material_color();
        color incoming_light = color(0, 0, 0);

        // Cast sample rays
        if (recur_depth < max_recur) {
            vec3 view_reflected = ray_direction + closest_intersect.normal * (vec3::dot(closest_intersect.normal, ray_direction) * -2);

            int sample_rays = std::max<int>(1, max_sample_rays * roughness);
            vec3 ray_vector, half_angle_vec;
            ray sample_ray;
            for (int i = 0; i < sample_rays; i++) {
                ray_vector = vec3(rand01() * 2 - 1, rand01() * 2 - 1, rand01() * 2 - 1).normalized() * roughness + view_reflected;
                ray_vector.normalize();
                if (vec3::dot(ray_vector, closest_intersect.normal) <= 0) {
                    continue;
                }
                sample_ray = ray(closest_intersect.position, ray_vector * 100);

                color sample_color = ray_color(sample_ray, recur_depth + 1, closest_intersect.shape_id);

                incoming_light += sample_color;
            }
            incoming_light /= sample_rays;

            // Light sources
            if (lights.size() > 0) {
                for (int i = 0; i < lights.size(); i++) {
                    const lightsource* light = lights[i];
                    vec3 vec_to_light = light->position() - closest_intersect.position;
                    float distance = vec_to_light.length();
                    float radius = light->radius();
                    float dot = vec3::dot(view_reflected, vec_to_light.normalized()) * roughness;
                    float strength = std::max<float>(0, dot * pow((radius - distance) / radius, light->falloff()) * light->intensity());
                    incoming_light += light->light_color() * strength;
                }
                incoming_light /= lights.size();
            }
        }

        float reflectivity = intersect_shape->shape_material().material_reflectivity();
        c = color(c.r() * incoming_light.r(), c.g() * incoming_light.g(), c.b() * incoming_light.b()) * reflectivity;
    } else {
        // Sky color
        c = color(0.8, 0.83, 1.0);
    }

    return c;
}

int main() {
    // Create objects
    sphere s1 = sphere(pos3( 0.1,  2.0, -2.0), 1, material(1.00, 1.0, color(0.4, 0.4, 0.9)));
    sphere s2 = sphere(pos3( 3.5,  2.0, -4.0), 1, material(0.01, 1.0, color(0.7, 0.9, 0.7)));
    sphere s3 = sphere(pos3(-1.0, -0.3, -5.0), 1, material(0.90, 1.0, color(0.9, 0.4, 0.4)));
    sphere s4 = sphere(pos3( 0.1,  1.0, -5.0), 1, material(0.10, 1.0, color(1.0, 0.5, 1.0)));
    sphere s5 = sphere(pos3(-3.0,  0.0, -4.0), 1, material(1.00, 1.0, color(1.0, 1.0, 1.0)));
    plane p1 = plane(pos3( 0.0, -1.5, -5.0), 100, 100, material(0.00, 1.0, color(0.8, 1.0, 0.8)));
    plane p2 = plane(pos3( 0.0,  0.5, -5.0),   3,   3, material(0.70, 1.0, color(0.9, 0.5, 0.5)));
    plane p3 = plane(pos3( 3.0,  0.5, -5.0),   3,   3, material(0.00, 1.0, color(1.0, 0.9, 0.9)));
    all_shapes = { &s1, &s2, &s3, &s4, &s5, &p1, &p2, &p3 };
    p2.set_orientation(quaternion::from_axis_angle(vec3(0, 1, 0), 30 * DEG_TO_RAD));
    p3.set_orientation(quaternion::from_axis_angle(vec3(1, 0, 0), 45 * DEG_TO_RAD));

    // Create lights
    lightsource l1 = lightsource(pos3(  0,   0,  -3), color(1.0, 1.0, 1.0), 10, 2, 1.0);
    lights = { &l1 };

    // Render image
    frame image = frame(image_height, image_width);

    std::cout << "Rendering image";
#ifdef MULTITHREADED
    int rows_per_thread = image_height / thread_count;
    std::vector<std::thread> threads = std::vector<std::thread>();
    for (int i = 0; i < thread_count; i++) {
        threads.push_back(std::thread([&image](int row_start, int count) {
            for (int pixel_y = row_start; pixel_y < row_start + count; pixel_y++) {
                for (int pixel_x = 0; pixel_x < image_width; pixel_x++) {
                    ray pixel_ray = cam.pixel_to_ray(pixel_x, pixel_y);
                    color pixel_color = color();
                    
                    for (int p = 0; p < cam_rays_per_pixel; p++) {
                        pixel_color += ray_color(pixel_ray, 0, -1)* color_max;
                    }
                    pixel_color /= cam_rays_per_pixel;

                    image.set_pixel(pixel_x, pixel_y, pixel_color);
                }
            }
#ifdef DISPLAY_PROGRESS
            std::cout << "\nThread completed: " << std::this_thread::get_id();
#endif
            }, i * rows_per_thread, rows_per_thread
        ));
    }

    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
#else
    std::cout << "\n";
    for (int pixel_y = 0; pixel_y < image_height; pixel_y++) {
        for (int pixel_x = 0; pixel_x < image_width; pixel_x++) {
            ray pixel_ray = cam.pixel_to_ray(pixel_x, pixel_y);
            color pixel_color = ray_color(pixel_ray, 0, -1) * color_max;

            image.set_pixel(pixel_x, pixel_y, pixel_color);
        }
#ifdef DISPLAY_PROGRESS
        std::cout << "\r" << float(pixel_y) / image_height << std::flush;
#endif
    }
#endif

    std::cout << "\nPerforming postprocessing";
    std::cout << "\n";
    // postprocessing::blur(image, 1);
    postprocessing::brightness_adjust(image, 10.0);

    std::cout << "\nWriting image to file";
    std::cout << "\n";
    image.write_to_file_bmp("image.bmp");

    return 0;
}
