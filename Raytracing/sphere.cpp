#include "sphere.h"

void sphere::set_radius(float radius) {
	r = radius;
    update_bounding_box();
}

shape::intersect_info sphere::ray_intersects(const ray& ray) const {
    vec3 oc = ray.origin() - pos;
    vec3 ray_direction = ray.direction();
    float a = ray_direction.length_squared();
    float half_b = vec3::dot(oc, ray_direction);
    float c = oc.length_squared() - r * r;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return intersect_info();
    } else {
        float distance = (-half_b - sqrt(discriminant)) / a;
        pos3 position = ray.at_distance(distance);
        vec3 vector_to_surface = position - pos;
        vec3 normal = vector_to_surface.normalized();

        return intersect_info(distance, position, normal, id);
    }
}

/// <summary>
/// Checks if a point is inside the sphere
/// </summary>
/// <param name="point">Point</param>
/// <returns>If point is inside sphere</returns>
bool sphere::is_inside(const pos3& point) const {
	float distance_sqr = (vec3(point) - pos).length_squared();
	return distance_sqr <= r * r;
}

void sphere::update_bounding_box() {
    vec3 rad = vec3(r, r, r);
    box = aabb(pos - rad, pos + rad);
}
