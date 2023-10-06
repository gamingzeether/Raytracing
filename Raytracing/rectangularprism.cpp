#include "rectangularprism.h"

static inline float min(float x, float y) {
	return x < y ? x : y;
}

static inline float max(float x, float y) {
	return x > y ? x : y;
}

// Modified from ray_intersects() in aabb
shape::intersect_info rectangularprism::ray_intersects(const ray& r) const {
	ray rayRot = ori.rotate(r, pos);
	const pos3& ray_origin = rayRot.origin();
	const vec3 ray_direction_inv = vec3(1 / rayRot.direction().x(), 1 / rayRot.direction().y(), 1 / rayRot.direction().z());
	float tmin = 0.0;
	float tmax = INFINITY;
	uint8_t face = 0;

	bool sign = signbit(ray_direction_inv[0]);
	float dmin = (corners[sign][0] - ray_origin[0]) * ray_direction_inv[0];
	float dmax = (corners[!sign][0] - ray_origin[0]) * ray_direction_inv[0];
	if (dmin > tmin) {
		tmin = dmin;
		face = sign * 8 + 1;
	}
	tmax = min(dmax, tmax);

	sign = signbit(ray_direction_inv[1]);
	dmin = (corners[sign][1] - ray_origin[1]) * ray_direction_inv[1];
	dmax = (corners[!sign][1] - ray_origin[1]) * ray_direction_inv[1];
	if (dmin > tmin) {
		tmin = dmin;
		face = sign * 8 + 2;
	}
	tmax = min(dmax, tmax);

	sign = signbit(ray_direction_inv[2]);
	dmin = (corners[sign][2] - ray_origin[2]) * ray_direction_inv[2];
	dmax = (corners[!sign][2] - ray_origin[2]) * ray_direction_inv[2];
	if (dmin > tmin) {
		tmin = dmin;
		face = sign * 8 + 4;
	}
	tmax = min(dmax, tmax);

	if (tmax >= tmin) {
		vec3 normal = vec3((face & 1) ? 1 : 0, (face & 2) ? 1 : 0, (face & 4) ? 1 : 0) * ((face & 8) ? 1 : -1);
		normal = ori.conjugate().rotate(normal);
		return intersect_info(tmin, r.at_distance(tmin), normal, id);
	} else {
		return intersect_info();
	}
}

void rectangularprism::update_bounding_box() {
	pos3 arr[8];
	for (uint8_t i = 0; i < 8; i++) {
		float x = (width / 2) * ((i & 1) ? 1 : -1);
		float y = (height / 2) * ((i & 2) ? 1 : -1);
		float z = (depth / 2) * ((i & 4) ? 1 : -1);
		arr[i] = pos + ori.rotate(pos3(x, y, z));
	}

	box = aabb(arr, 8);
}
