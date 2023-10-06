#include "aabb.h"

static inline float min(float x, float y) {
	return x < y ? x : y;
}

static inline float max(float x, float y) {
	return x > y ? x : y;
}

inline bool between(float low, float high, float test) {
	return test >= low && test <= high;
}

aabb::aabb(const pos3* points, int count) {
	float min_x = INFINITY;
	float min_y = INFINITY;
	float min_z = INFINITY;
	float max_x = -INFINITY;
	float max_y = -INFINITY;
	float max_z = -INFINITY;

	for (int i = 0; i < count; i++) {
		const pos3& point = points[i];
		min_x = min(min_x, point.x());
		min_y = min(min_y, point.y());
		min_z = min(min_z, point.z());
		max_x = max(max_x, point.x());
		max_y = max(max_y, point.y());
		max_z = max(max_z, point.z());
	}

	corners[0] = pos3(min_x, min_y, min_z);
	corners[1] = pos3(max_x, max_y, max_z);
}

bool aabb::point_inside(const pos3& point) const {
	return between(corners[0].x(), corners[1].x(), point.x()) &&
		between(corners[0].y(), corners[1].y(), point.y()) &&
		between(corners[0].z(), corners[1].z(), point.z());
}

bool aabb::intersects_aabb(const aabb& other) const {
	return (corners[0].x() <= other.corners[1].x() && corners[1].x() >= other.corners[0].x()) &&
		(corners[0].y() <= other.corners[1].y() && corners[1].y() >= other.corners[0].y()) &&
		(corners[0].z() <= other.corners[1].z() && corners[1].z() >= other.corners[0].z());
}

// https://tavianator.com/2011/ray_box.html, https://tavianator.com/2022/ray_box_boundary.html
bool aabb::ray_intersects(const ray& r) const {
	const pos3& ray_origin = r.origin();
	const vec3 ray_direction_inv = vec3(1 / r.direction().x(), 1 / r.direction().y(), 1 / r.direction().z());
	float tmin = 0.0;
	float tmax = INFINITY;

	bool sign = signbit(ray_direction_inv[0]);
	float dmin = (corners[sign][0] - ray_origin[0]) * ray_direction_inv[0];
	float dmax = (corners[!sign][0] - ray_origin[0]) * ray_direction_inv[0];
	tmin = max(dmin, tmin);
	tmax = min(dmax, tmax);
	
	sign = signbit(ray_direction_inv[1]);
	dmin = (corners[sign][1] - ray_origin[1]) * ray_direction_inv[1];
	dmax = (corners[!sign][1] - ray_origin[1]) * ray_direction_inv[1];
	tmin = max(dmin, tmin);
	tmax = min(dmax, tmax);
	
	sign = signbit(ray_direction_inv[2]);
	dmin = (corners[sign][2] - ray_origin[2]) * ray_direction_inv[2];
	dmax = (corners[!sign][2] - ray_origin[2]) * ray_direction_inv[2];
	tmin = max(dmin, tmin);
	tmax = min(dmax, tmax);

	return tmax >= tmin;
}
