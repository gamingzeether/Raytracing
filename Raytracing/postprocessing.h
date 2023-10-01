#pragma once

#include "frame.h"

/// <summary>
/// Static class that performs postprocessing on an image
/// </summary>
class postprocessing {
public:
	static void blur(frame& image, int radius, float strength);
	static void brightness_adjust(frame& image, float brightness);
	static void noise_reduction(frame& image, const array2d<int>& shape_id, float strength);
private:
};
