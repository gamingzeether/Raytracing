#include "aabb.h"

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
