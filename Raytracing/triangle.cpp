#include "triangle.h"

void triangle::set_point(int i, vec3 p) {
	points[i] = p;
}

void triangle::set_point(int i, vec2 p) {
    points[i] = vec3(p);
}

// Möller–Trumbore intersection algorithm
// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm#C++_implementation
shape::intersect_info triangle::ray_intersects(const ray& r) const {
    // Keep consistiency w/ other rotations; otherwise it would rotate the opposite way to a plane w/ same orientation
    ray rotated_ray = ori.rotate(r, pos);

    const float EPSILON = 0.0000001;
    vec3 vertex0 = pos + points[0];
    vec3 vertex1 = pos + points[1];
    vec3 vertex2 = pos + points[2];
    vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = vec3::cross(rotated_ray.direction(), edge2);
    a = vec3::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return intersect_info();    // This ray is parallel to this triangle.

    f = 1.0 / a;
    s = rotated_ray.origin() - vertex0;
    u = f * vec3::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return intersect_info();

    q = vec3::cross(s, edge1);
    v = f * vec3::dot(rotated_ray.direction(), q);

    if (v < 0.0 || u + v > 1.0)
        return intersect_info();

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * vec3::dot(edge2, q);

    if (t > EPSILON) // ray intersection
    {
        vec3 outIntersectionPoint = r.origin() + r.direction() * t;

        vec3 normal = vec3(0, -rotated_ray.direction().y(), 0);
        normal.normalize();
        normal = ori.conjugate().rotate(normal);

        return intersect_info(t, outIntersectionPoint, normal, id);
    } else // This means that there is a line intersection but not a ray intersection.
        return intersect_info();
}

bool triangle::is_inside(const pos3& point) const {
    return false;
}

void triangle::update_bounding_box() {
    vec4 ori_conjugate = ori.conjugate();
    vec3 real_points[3] = { pos + ori_conjugate.rotate(points[0]), pos + ori_conjugate.rotate(points[1]), pos + ori_conjugate.rotate(points[2]) };
    box = aabb(real_points, 3);
}
