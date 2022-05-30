#include "lightsource.h"

void lightsource::set_position(pos3 position) {
	pos = position;
}

void lightsource::set_light_color(color c) {
	col = c;
}

void lightsource::set_radius(float radius) {
	r = radius;
}

void lightsource::set_falloff(float falloff) {
	f = falloff;
}

void lightsource::set_intensity(float intensity) {
	i = intensity;
}
