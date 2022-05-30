#pragma once

#include "ray.h"

class lightsource {
public:
	lightsource() : pos(pos3(0, 0, 0)), r(1), f(1), i(1) {}
	lightsource(pos3 position, color c, float radius, float falloff, float intensity) : pos(position), col(c), r(radius), f(falloff), i(intensity) {}

	inline pos3 position() const { return pos; }
	void set_position(pos3 position);
	inline color light_color() const { return col; }
	void set_light_color(color c);
	inline float radius() const { return r; }
	void set_radius(float radius);
	inline float falloff() const { return f; }
	void set_falloff(float falloff);
	inline float intensity() const { return i; }
	void set_intensity(float intensity);
protected:
	pos3 pos;
	color col;
	float r;
	float f;
	float i;
};

