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

shape::intersect_info plane::ray_intersects(const ray& r) const {
	ray rayRot = ori.rotate(r, pos);

	// Check x and z when ray crosses plane
	float dist = (pos.y() - rayRot.origin().y()) / rayRot.direction().y();
	if (dist < 0 || dist > rayRot.direction().length()) {
		return intersect_info();
	}
	pos3 potential_intersect_pos = rayRot.at_distance(dist);

	if (abs(potential_intersect_pos.x() - pos.x()) < half_w && abs(potential_intersect_pos.z() - pos.z()) < half_h) {
		vec3 normal = vec3(0, -rayRot.direction().y(), 0);
		normal.normalize();
		normal = ori.conjugate().rotate(normal);
		potential_intersect_pos = ori.conjugate().rotate(potential_intersect_pos - pos) + pos;

		return intersect_info(dist, potential_intersect_pos, normal, id);
	} else {
		return intersect_info();
	}
}

bool plane::is_inside(const pos3& point) const {
	return false;
}

void plane::update_bounding_box() {
	pos3 p1 = pos + ori.rotate(pos3(half_w, 0, half_h));
	pos3 p2 = pos + ori.rotate(pos3(half_w, 0, -half_h));
	pos3 p3 = pos + ori.rotate(pos3(-half_w, 0, half_h));
	pos3 p4 = pos + ori.rotate(pos3(-half_w, 0, -half_h));
	const pos3 arr[4] = { p1, p2, p3, p4 };

	box = aabb(arr, 4);
}
