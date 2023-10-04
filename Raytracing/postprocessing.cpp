#include "postprocessing.h"

inline float min(float x, float y) {
	return (x < y) ? x : y;
}

inline float max(float x, float y) {
	return (x > y) ? x : y;
}

void postprocessing::process(frame& image, const array2d<int>& shape_id, const postprocessing_settings& settings) {
	if (!(settings.blur_enabled || settings.brightness_enabled || settings.noise_enabled)) {
		return;
	}

	int height = image.height();
	int width = image.width();

	frame temp = frame(height, width);
	array2d<int> luminance = array2d<int>(height, width);

	// Noise luminance
	if (settings.noise_enabled) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				const color c = image.get_pixel(i, j);
				int pix_luminance = 0.2126 * c.r() + 0.7152 * c.g() + 0.0722 * c.b();
				luminance.set(i, j, pix_luminance);
			}
		}
	}

	for (int pixel_x = 0; pixel_x < width; pixel_x++) {
		for (int pixel_y = 0; pixel_y < height; pixel_y++) {
			color c = color();

			// Blur
			if (settings.blur_enabled) {
				int sampledPixels = 0;
				for (int i = -settings.blur_radius; i <= settings.blur_radius; i++) {
					for (int j = -settings.blur_radius; j <= settings.blur_radius; j++) {
						int x = pixel_x + i;
						int y = pixel_y + j;
						if (x >= 0 && x < width && y >= 0 && y < height) {
							c += image.get_pixel(x, y);
							sampledPixels++;
						}
					}
				}
				c /= sampledPixels;
				c = color::lerp(image.get_pixel(pixel_x, pixel_y), c, settings.blur_strength);
			} else {
				c = image.get_pixel(pixel_x, pixel_y);
			}

			// Brightness
			if (settings.brightness_enabled) {
				c = color(max(0, min(COLOR_MAX, c.r() + settings.brightness_adjust)), max(0, min(COLOR_MAX, c.g() + settings.brightness_adjust)), max(0, min(COLOR_MAX, c.b() + settings.brightness_adjust)));
			}

			// Noise
			if (settings.noise_enabled) {
				int pixel_shape_id = shape_id.get(pixel_x, pixel_y);
				if (pixel_shape_id != -1) {
					float avg_luminance = 0;
					float sampled_pixels = 0;
					for (int i = -settings.noise_radius; i <= settings.noise_radius; i++) {
						for (int k = -settings.noise_radius; k <= settings.noise_radius; k++) {
							int x = pixel_x + i;
							int y = pixel_y + k;
							if (x >= 0 && x < width && y >= 0 && y < height && pixel_shape_id == shape_id.get(x, y)) {
								int dist = abs(i) + abs(k);
								float scale = pow(0.75, dist);
								avg_luminance += luminance.get(x, y) * scale;
								sampled_pixels += scale;
							}
						}
					}
					avg_luminance = avg_luminance / sampled_pixels;

					float diff = (avg_luminance - luminance.get(pixel_x, pixel_y)) * settings.noise_strength;
					float r = max(0, min(COLOR_MAX, c.r() + diff));
					float g = max(0, min(COLOR_MAX, c.g() + diff));
					float b = max(0, min(COLOR_MAX, c.b() + diff));
					c = vec3(r, g, b);
				}
			}

			temp.set_pixel(pixel_x, pixel_y, c);
		}
	}
	image = temp;
}
