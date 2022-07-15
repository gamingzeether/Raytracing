#pragma once

#include "frame.h"

/// <summary>
/// Static class that performs postprocessing on an image
/// </summary>
class postprocessing {
public:
	static void blur(frame& image, int radius);
	static void brightness_adjust(frame& image, float brightness);
private:
};
