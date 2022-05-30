#include "shape.h"

int shape::next_id = 0;

void shape::set_position(pos3 new_pos) {
	pos = new_pos;
	update_bounding_box();
}

void shape::set_shape_material(material mat) {
	m = mat;
}

void shape::set_bounding_box(aabb bounding_box) {
	box = bounding_box;
}

shape::intersect_info shape::ray_intersects(const ray& ray) const {
	throw;
	return intersect_info();
}

bool shape::is_inside(const pos3& point) const {
	throw;
	return false;
}

void shape::update_bounding_box() {
	throw;
}
