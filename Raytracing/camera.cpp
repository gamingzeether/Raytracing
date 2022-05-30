#include "camera.h"

pos3 camera::position() {
	return pos;
}

void camera::set_position(pos3 position) {
	pos = position;
}

ray camera::pixel_to_ray(int x, int y) const {
	vec3 lower_left_corner = pos - vec3(viewport_width / 2, viewport_height / 2, focal_length);
	return ray(pos, lower_left_corner + vec3((double(x) / image_width) * viewport_width, (double(y) / image_height) * viewport_height, 0) - pos);
}