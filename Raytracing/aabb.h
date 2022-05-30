#pragma once

#include <vector>

#include "vec3.h"

class aabb {
public:
	aabb() : position_min(pos3(0, 0, 0)), position_max(pos3(0, 0, 0)) {}
	aabb(pos3 min, pos3 max) : position_min(min), position_max(max) {}
	aabb(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z) : position_min(pos3(min_x, min_y, min_z)), position_max(pos3(max_x, max_y, max_z)) {}
	aabb(const std::vector<pos3>& points) {
		float min_x = 0;
		float min_y = 0;
		float min_z = 0;
		float max_x = 0;
		float max_y = 0;
		float max_z = 0;

		for (const pos3& point : points) {
			if (point.x() < min_x) {
				min_x = point.x();
			} else if (point.x() > max_x) {
				max_x = point.x();
			}
			if (point.y() < min_y) {
				min_y = point.y();
			} else if (point.y() > max_y) {
				max_y = point.y();
			}
			if (point.z() < min_z) {
				min_z = point.z();
			} else if (point.z() > max_z) {
				max_z = point.z();
			}
		}

		position_min = pos3(min_x, min_y, min_z);
		position_max = pos3(max_x, max_y, max_z);
	}

	bool point_inside(const pos3& point) const;
	bool intersects_aabb(const aabb& other) const;
protected:
	pos3 position_min;
	pos3 position_max;
};

