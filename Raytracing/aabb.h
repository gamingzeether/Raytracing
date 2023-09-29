#pragma once

#include <vector>

#include "ray.h"

class aabb {
public:
	aabb() : corners { pos3(0, 0, 0), pos3(0, 0, 0) } {}
	aabb(pos3 min, pos3 max) : corners { min, max } {}
	aabb(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z) : corners { pos3(min_x, min_y, min_z), pos3(max_x, max_y, max_z) } {}
	aabb(const pos3* points, int count);

	bool point_inside(const pos3& point) const;
	bool intersects_aabb(const aabb& other) const;
	bool ray_intersects(const ray& r) const;
protected:
	pos3 corners[2];
};

