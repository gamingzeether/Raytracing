#include "plane.h"

void plane::set_width(float width) {
	w = width;
	half_w = width / 2;
	update_bounding_box();
}

void plane::set_height(float height) {
	h = height;
	half_h = height / 2;
	update_bounding_box();
}

shape::intersect_info plane::ray_intersects(const ray& ray) const {
	// Check x and z when ray crosses plane
	float dist = (pos.y() - ray.origin().y()) / ray.direction().y();
	if (dist < 0 || dist > ray.direction().length()) {
		return intersect_info();
	}
	pos3 potential_intersect_pos = ray.at_distance(dist);

	if (abs(potential_intersect_pos.x() - pos.x()) < half_w && abs(potential_intersect_pos.z() - pos.z()) < half_h) {
		vec3 normal = vec3(0, -ray.direction().y(), 0);
		normal.normalize();

		return intersect_info(dist, potential_intersect_pos, normal, id);
	} else {
		return intersect_info();
	}
}

bool plane::is_inside(const pos3& point) const {
	return false;
}

void plane::update_bounding_box() {
	vec3 vec = vec3(half_w, 0, half_h);
	box = aabb(pos - vec, pos + vec);
}
