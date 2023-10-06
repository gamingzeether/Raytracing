#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "aabb.h"
#include "camera.h"
#include "frame.h"
#include "lightsource.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "shape.h"
#include "triangle.h"
#include "postprocessing.h"
#include "rectangularprism.h"

// Settings defines
 #define DISPLAY_PROGRESS
// #define HIGH_QUALITY
// #define LOW_QUALITY

#ifdef HIGH_QUALITY
#undef LOW_QUALITY
#endif

// Define constants
#define PI 3.1415926535897932385
#define DEG_TO_RAD (PI / 180)
#define RAD_TO_DEG (180 / PI)

#ifdef LOW_QUALITY
const int max_recur = 3;
const int image_width = 512;
const int image_height = 512;
const int max_sample_rays = 3;
#else
#ifdef HIGH_QUALITY
const int max_recur = 10;
const int image_width = 4096;
const int image_height = 4096;
const int max_sample_rays = 3;
#else
const int max_recur = 3;
const int image_width = 1024;
const int image_height = 1024;
const int max_sample_rays = 3;
#endif
#endif
const int color_max = 255;
const int cam_rays_per_pixel = 3;
const int thread_count = 16;

camera cam = camera(image_width / image_height, image_height, 20, 10);
std::vector<lightsource*> lights;
std::vector<shape*> all_shapes;

vec3* pregenerated_random_vectors;
const int pregenerated_random_vectors_size = 512;

inline double rand01() {
    return double(rand()) / RAND_MAX;
}

color ray_color(const ray& light_ray, int recur_depth, int ignore, OUT int& shape_id) {
    vec3 ray_direction = light_ray.direction().normalized();

    // Check if ray intersects with any shapes
    shape::intersect_info closest_intersect = shape::intersect_info();
    for (int i = 0; i < all_shapes.size(); i++) {
        if (i == ignore) {
            continue;
        }
        const shape* s = all_shapes[i];

        if (!s->bounding_box().ray_intersects(light_ray)) {
            continue;
        }

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
        shape_id = closest_intersect.shape_id;
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
                ray_vector = pregenerated_random_vectors[rand() % pregenerated_random_vectors_size] * roughness + view_reflected;
                ray_vector.normalize();
                if (vec3::dot(ray_vector, closest_intersect.normal) <= 0) {
                    continue;
                }
                sample_ray = ray(closest_intersect.position, ray_vector * 100);

                int secondary_shape_id = -1;
                color sample_color = ray_color(sample_ray, recur_depth + 1, closest_intersect.shape_id, secondary_shape_id);

                incoming_light += sample_color;

                if (intersect_shape->shape_material().material_roughness() <= 0.05) {
                    shape_id = secondary_shape_id;
                }
            }
            incoming_light /= sample_rays;

            // Light sources
            if (lights.size() > 0) {
                for (int i = 0; i < lights.size(); i++) {
                    const lightsource* light = lights[i];
                    vec3 vec_to_light = light->position() - closest_intersect.position;
                    float distance = vec_to_light.length();
                    float radius = light->radius();
                    float dot = vec3::dot(closest_intersect.normal, vec_to_light.normalized());
                    float strength = std::max<float>(0, dot * roughness * pow((radius - distance) / radius, light->falloff()) * light->intensity());
                    incoming_light += light->light_color() * strength;
                }
                incoming_light /= lights.size();
            }
        }

        float reflectivity = intersect_shape->shape_material().material_reflectivity();
        c = color(c.r() * incoming_light.r(), c.g() * incoming_light.g(), c.b() * incoming_light.b()) * reflectivity;
    } else {
        shape_id = -1;
        // Sky color
        c = color(0.8, 0.8, 0.8);
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
    triangle t1 = triangle(pos3(-3.0, 2.0, -5.0), vec3(0, 0, 2), vec3(-1, 0, 0), vec3(1, 0, 0), material(0.00, 1.0, color(0.4, 0.4, 0.4)));
    rectangularprism rp1 = rectangularprism(pos3(-3.0, 1.0, -4), 0.5, 0.5, 0.5, material(0.00, 0.8, color(1.0, 1.0, 1.0)));
    p2.set_orientation(quaternion::from_axis_angle(vec3(0, 1, 0), 30 * DEG_TO_RAD));
    p3.set_orientation(quaternion::from_axis_angle(vec3(1, 0, 0), -45 * DEG_TO_RAD));
    t1.set_orientation(quaternion::from_axis_angle(vec3(1, 0, 0), 45 * DEG_TO_RAD));
    rp1.set_orientation(quaternion::from_axis_angle(vec3(0, 1, 0), -45 * DEG_TO_RAD));
    all_shapes = { &s1, &s2, &s3, &s4, &s5, &p1, &p2, &p3, &t1, &rp1};

    // Create lights
    lightsource l1 = lightsource(pos3(  0,   0,  -3), color(1.0, 1.0, 1.0), 10, 1, 0.75);
    lights = { &l1 };

    // Render image
    frame image = frame(image_height, image_width);
    array2d<int> pixel_shape = array2d<int>(image_height, image_width);

    std::cout << "Rendering image\n";

    std::vector<std::thread> threads = std::vector<std::thread>();
    // Assigner thread
    bool threads_working = true;
    std::vector<int> assignment_map = std::vector<int>(); // scaled down 1D representation of image
    threads.push_back(std::thread([&image, &assignment_map, &threads_working](int width, int height) {
        // -2 = unassigned, -1 = complete, other = assigned thread
        for (int i = 0; i < (thread_count * 2); i++) {
            for (int j = 0; j < (thread_count * 2); j++) {
                assignment_map.push_back(-2);
            }
        }

        int count[thread_count];
        bool working = true;
        while (working) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            for (int i = 0; i < thread_count; i++) {
                count[i] = 0;
            }
            // Check assignment map for completion or find threads that need assigning
            working = false;
            int count_complete = 0;
            for (int i = 0; i < assignment_map.size(); i++) {
                if (assignment_map[i] != -1) {
                    working = true;
                } else {
                    count_complete++;
                }
                count[assignment_map[i]]++;
            }
#ifdef DISPLAY_PROGRESS
            std::cout << "\r" << (int)((float(count_complete) / assignment_map.size()) * 100) << std::flush;
#endif

            // Assign chunks to threads
            for (int i = 0; i < assignment_map.size(); i++) {
                if (assignment_map[i] == -2) {
                    for (int j = 0; j < thread_count; j++) {
                        if (count[j] < 8) {
                            assignment_map[i] = j;
                            count[j]++;
                            break;
                        }
                    }
                }
                if (count[thread_count - 1] == 8) {
                    break;
                }
            }
        }
        threads_working = false;
        }, image_width, image_height
    ));

    pregenerated_random_vectors = new vec3[pregenerated_random_vectors_size];
    for (int i = 0; i < pregenerated_random_vectors_size; i++) {
        pregenerated_random_vectors[i] = vec3(rand01() * 2 - 1, rand01() * 2 - 1, rand01() * 2 - 1).normalized();
    }

    const int chunk_width = image_width / (thread_count * 2);
    const int chunk_height = image_height / (thread_count * 2);
    for (int i = 0; i < thread_count; i++) {
        threads.push_back(std::thread([&image, &assignment_map, &threads_working, &pixel_shape](int id) {
            while (threads_working) {
                for (int i = 0; i < assignment_map.size(); i++) {
                    if (assignment_map[i] == id) {
                        // Calculate starting location
                        int start_x = (i % (thread_count * 2)) * chunk_width;
                        int start_y = (i / (thread_count * 2)) * chunk_height;
                        // Render
                        for (int offset_x = 0; offset_x < chunk_width; offset_x++) {
                            for (int offset_y = 0; offset_y < chunk_height; offset_y++) {
                                int pixel_x = start_x + offset_x;
                                int pixel_y = start_y + offset_y;

                                ray pixel_ray = cam.pixel_to_ray(pixel_x, pixel_y);
                                color pixel_color = color();

                                int pixel_shape_id = -1;
                                for (int p = 0; p < cam_rays_per_pixel; p++) {
                                    pixel_color += ray_color(pixel_ray, 0, -1, OUT pixel_shape_id) * color_max;
                                }
                                pixel_color /= cam_rays_per_pixel;

                                image.set_pixel(pixel_x, pixel_y, pixel_color);
                                pixel_shape.set(pixel_x, pixel_y, pixel_shape_id);
                            }
                        }
                        // Mark complete
                        assignment_map[i] = -1;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            }, i
        ));
    }

    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\nPerforming postprocessing";
    std::cout << "\n";
    postprocessing_settings pp_settings;
    pp_settings.noise_enabled = true;
    pp_settings.noise_luminance_radius = 3;
    pp_settings.noise_luminance_strength = 0.25;
    pp_settings.noise_chroma_radius = 3;
    pp_settings.noise_chroma_strength = 0.75;
    pp_settings.blur_enabled = true;
    pp_settings.blur_radius = 2;
    pp_settings.blur_strength = 0.25;
    pp_settings.brightness_enabled = true;
    pp_settings.brightness_adjust = 0.04;
    postprocessing::process(image, pixel_shape, pp_settings);

    std::cout << "\nWriting image to file";
    std::cout << "\n";
    image.write_to_file_bmp("image.bmp");

    return 0;
}
