#include "ray.h"

pos3 ray::origin() const {
	return orig;
}

vec3 ray::direction() const {
	return dir;
}

pos3 ray::at_distance(float d) const {
	return orig + dir * d;
}
