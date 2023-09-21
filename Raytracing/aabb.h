#pragma once

#include <vector>

#include "ray.h"

class aabb {
public:
	aabb() : position_min(pos3(0, 0, 0)), position_max(pos3(0, 0, 0)) {}
	aabb(pos3 min, pos3 max) : position_min(min), position_max(max) {}
	aabb(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z) : position_min(pos3(min_x, min_y, min_z)), position_max(pos3(max_x, max_y, max_z)) {}
	aabb(const pos3* points, int count);

	bool point_inside(const pos3& point) const;
	bool intersects_aabb(const aabb& other) const;
	bool ray_intersects(const ray& r) const;
protected:
	pos3 position_min;
	pos3 position_max;
};

