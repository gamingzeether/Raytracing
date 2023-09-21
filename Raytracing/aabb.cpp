#include "aabb.h"

aabb::aabb(const pos3* points, int count) {
	float min_x = INFINITY;
	float min_y = INFINITY;
	float min_z = INFINITY;
	float max_x = -INFINITY;
	float max_y = -INFINITY;
	float max_z = -INFINITY;

	for (int i = 0; i < count; i++) {
		const pos3& point = points[i];
		if (point.x() < min_x) {
			min_x = point.x();
		}
		if (point.x() > max_x) {
			max_x = point.x();
		}
		if (point.y() < min_y) {
			min_y = point.y();
		}
		if (point.y() > max_y) {
			max_y = point.y();
		}
		if (point.z() < min_z) {
			min_z = point.z();
		}
		if (point.z() > max_z) {
			max_z = point.z();
		}
	}

	position_min = pos3(min_x, min_y, min_z);
	position_max = pos3(max_x, max_y, max_z);
}

inline bool between(float low, float high, float test) {
	return test >= low && test <= high;
}

bool aabb::point_inside(const pos3& point) const {
	return between(position_min.x(), position_max.x(), point.x()) &&
		between(position_min.y(), position_max.y(), point.y()) &&
		between(position_min.z(), position_max.z(), point.z());
}

bool aabb::intersects_aabb(const aabb& other) const {
	return (position_min.x() <= other.position_max.x() && position_max.x() >= other.position_min.x()) &&
		(position_min.y() <= other.position_max.y() && position_max.y() >= other.position_min.y()) &&
		(position_min.z() <= other.position_max.z() && position_max.z() >= other.position_min.z());
}

// https://tavianator.com/2011/ray_box.html
bool aabb::ray_intersects(const ray& r) const {
	const pos3& ray_origin = r.origin();
	const vec3 ray_direction_inv = vec3(1 / r.direction().x(), 1 / r.direction().y(), 1 / r.direction().z());

	float tx1 = (position_min.x() - ray_origin.x()) * ray_direction_inv.x();
	float tx2 = (position_max.x() - ray_origin.x()) * ray_direction_inv.x();
	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);
	if (tmax < tmin) {
		return false;
	}

	float ty1 = (position_min.y() - ray_origin.y()) * ray_direction_inv.y();
	float ty2 = (position_max.y() - ray_origin.y()) * ray_direction_inv.y();
	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));
	if (tmax < tmin) {
		return false;
	}

	float tz1 = (position_min.z() - ray_origin.z()) * ray_direction_inv.z();
	float tz2 = (position_max.z() - ray_origin.z()) * ray_direction_inv.z();
	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	return tmax >= tmin;
}
