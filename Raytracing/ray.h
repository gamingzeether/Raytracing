#pragma once

#include "vec3.h"

class ray {
public:
	ray() {}
	ray(const pos3& o, const vec3& d) : orig(o), dir(d) {}

	pos3 origin() const;
	vec3 direction() const;

	pos3 at_distance(float d) const;
private:
	pos3 orig;
	vec3 dir;
};

