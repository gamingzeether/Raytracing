#include "sphere.h"

void sphere::set_radius(float radius) {
	rad = radius;
    update_bounding_box();
}

shape::intersect_info sphere::ray_intersects(const ray& r) const {
    vec3 oc = r.origin() - pos;
    vec3 ray_direction = r.direction();
    float a = ray_direction.length_squared();
    float half_b = vec3::dot(oc, ray_direction);
    float c = oc.length_squared() - rad * rad;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return intersect_info();
    } else {
        float distance = (-half_b - sqrt(discriminant)) / a;
        pos3 position = r.at_distance(distance);
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
	return distance_sqr <= rad * rad;
}

void sphere::update_bounding_box() {
    vec3 vecRad = vec3(rad, rad, rad);
    box = aabb(pos - vecRad, pos + vecRad);
}
