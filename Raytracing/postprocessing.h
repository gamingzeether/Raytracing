#pragma once

#include "frame.h"
#include "matrix.h"

struct postprocessing_settings {
	bool noise_enabled;
	float noise_luminance_strength;
	int noise_luminance_radius;
	float noise_chroma_strength;
	int noise_chroma_radius;

	bool blur_enabled;
	int blur_radius;
	float blur_strength;

	bool brightness_enabled;
	float brightness_adjust;
};

/// <summary>
/// Static class that performs postprocessing on an image
/// </summary>
class postprocessing {
public:
	static void process(frame& image, const array2d<int>& shape_id, const postprocessing_settings& settings);
};
