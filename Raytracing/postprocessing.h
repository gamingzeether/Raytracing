#pragma once

#include "frame.h"

struct postprocessing_settings {
	bool blur_enabled;
	int blur_radius;
	float blur_strength;

	bool brightness_enabled;
	float brightness_adjust;

	bool noise_enabled;
	float noise_strength;
	int noise_radius;
};

/// <summary>
/// Static class that performs postprocessing on an image
/// </summary>
class postprocessing {
public:
	static void process(frame& image, const array2d<int>& shape_id, const postprocessing_settings& settings);
};
