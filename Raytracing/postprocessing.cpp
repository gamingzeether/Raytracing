#include "postprocessing.h"

void postprocessing::blur(frame& image, int radius, float strength) {
	frame temp = frame(image.height(), image.width());
	for (int i = 0; i < image.width(); i++) {
		for (int j = 0; j < image.height(); j++) {
			color c = color();
			int sampledPixels = 0;
			for (int k = -radius; k <= radius; k++) {
				for (int l = -radius; l <= radius; l++) {
					int x = i + k;
					int y = j + l;
					if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
						c += image.get_pixel(x, y);
						sampledPixels++;
					}
				}
			}
			c /= sampledPixels;
			temp.set_pixel(i, j, color::lerp(image.get_pixel(i, j), c, strength));
		}
	}
	image = temp;
}

void postprocessing::brightness_adjust(frame& image, float brightness) {
	color c;
	for (int i = 0; i < image.width(); i++) {
		for (int j = 0; j < image.height(); j++) {
			c = image.get_pixel(i, j);
			image.set_pixel(i, j, color(fmax(0, fmin(COLOR_MAX, c.r() + brightness)), fmax(0, fmin(COLOR_MAX, c.g() + brightness)), fmax(0, fmin(COLOR_MAX, c.b() + brightness))));
		}
	}
}

inline float min(float x, float y) {
	return (x < y) ? x : y;
}

inline float max(float x, float y) {
	return (x > y) ? x : y;
}

void postprocessing::noise_reduction(frame& image, const array2d<int>& shape_id, float strength) {
	const int height = image.height();
	const int width = image.width();

	array2d<int> luminance = array2d<int>(height, width);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			const color& c = image.get_pixel(x, y);
			int pix_luminance = 0.2126 * c.r() + 0.7152 * c.g() + 0.0722 * c.b();
			luminance.set(x, y, pix_luminance);
		}
	}
	const int radius = 5;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int pixel_shape_id = shape_id.get(i, j);
			if (pixel_shape_id == -1) {
				continue;
			}

			float avg_luminance = 0;
			float sampled_pixels = 0;
			for (int k = -radius; k <= radius; k++) {
				for (int l = -radius; l <= radius; l++) {
					int x = i + k;
					int y = j + l;
					if (x >= 0 && x < width && y >= 0 && y < height && pixel_shape_id == shape_id.get(x, y)) {
						int dist = abs(k) + abs(l);
						float scale = pow(0.75, dist);
						avg_luminance += luminance.get(x, y) * scale;
						sampled_pixels += scale;
					}
				}
			}
			avg_luminance = avg_luminance / (float)sampled_pixels;

			float diff = (avg_luminance - luminance.get(i, j)) * strength;
			const color& original_color = image.get_pixel(i, j);
			float r = max(0, min(COLOR_MAX, original_color.r() + diff));
			float g = max(0, min(COLOR_MAX, original_color.g() + diff));
			float b = max(0, min(COLOR_MAX, original_color.b() + diff));
			image.set_pixel(i, j, vec3(r, g, b));
		}
	}
}
