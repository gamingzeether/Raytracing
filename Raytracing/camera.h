#pragma once

#include "ray.h"

class camera {
public:
	camera() : aspect_ratio(16 / 9), image_height(1980), viewport_height(2.0f), focal_length(1.0f) {
		pos = pos3(0, 0, 0);
		image_width = image_height * aspect_ratio;
		viewport_width = viewport_height * aspect_ratio;
	}
	camera(float aspect, int height, float v_height, float focal) : aspect_ratio(aspect), image_height(height), viewport_height(v_height), focal_length(focal) {
		pos = pos3(0, 0, 0);
		image_width = image_height * aspect_ratio;
		viewport_width = viewport_height * aspect_ratio;
	}

	pos3 position();
	void set_position(pos3 position);

	ray pixel_to_ray(int x, int y) const;
protected:
	pos3 pos;
	float aspect_ratio;
	int image_width;
	int image_height;
	float viewport_height;
	float viewport_width;
	float focal_length;
};
