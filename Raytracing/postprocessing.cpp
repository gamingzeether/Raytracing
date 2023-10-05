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

	array2d<color_ycbcr> ycbcr = array2d<color_ycbcr>(height, width);

	const float Kr = 0.2126;
	const float Kg = 0.7152;
	const float Kb = 0.0722;
	// RGB to YCbCr matrix
	matrix color_matrix = matrix(3, 3);
	color_matrix.set(0, 0, Kr);
	color_matrix.set(1, 0, Kg);
	color_matrix.set(2, 0, Kb);
	color_matrix.set(0, 1, (-1.0 / 2.0) * (Kr / (1.0 - Kb)));
	color_matrix.set(1, 1, (-1.0 / 2.0) * (Kg / (1.0 - Kb)));
	color_matrix.set(2, 1, 1.0 / 2.0);
	color_matrix.set(0, 2, 1.0 / 2.0);
	color_matrix.set(1, 2, (-1.0 / 2.0) * (Kg / (1.0 - Kr)));
	color_matrix.set(2, 2, (-1.0 / 2.0) * (Kb / (1.0 - Kr)));

	// YCbCr to RGB matrix
	matrix color_matrix_inverse = matrix(3, 3);
	color_matrix_inverse.set(0, 0, 1);
	color_matrix_inverse.set(1, 0, 0);
	color_matrix_inverse.set(2, 0, 2 - 2 * Kr);
	color_matrix_inverse.set(0, 1, 1);
	color_matrix_inverse.set(1, 1, (-Kb / Kg) * (2 - 2 * Kb));
	color_matrix_inverse.set(2, 1, (-Kr / Kg) * (2 - 2 * Kr));
	color_matrix_inverse.set(0, 2, 1);
	color_matrix_inverse.set(1, 2, 2 - 2 * Kb);
	color_matrix_inverse.set(2, 2, 0);

	// Convert to YCbCr
	for (int pixel_x = 0; pixel_x < width; pixel_x++) {
		for (int pixel_y = 0; pixel_y < height; pixel_y++) {
			matrix color = matrix(image.get_pixel(pixel_x, pixel_y));
			color = color.scalar_multiply(1.0 / COLOR_MAX);
			ycbcr.set(pixel_x, pixel_y, (color_matrix.matrix_multiply(color)).vector3());
		}
	}

	for (int pixel_x = 0; pixel_x < width; pixel_x++) {
		for (int pixel_y = 0; pixel_y < height; pixel_y++) {
			color c = ycbcr.get(pixel_x, pixel_y);

			// Noise
			if (settings.noise_enabled) {
				int pixel_shape_id = shape_id.get(pixel_x, pixel_y);
				if (pixel_shape_id != -1) {
					// Luminance
					float avg_luminance = 0;
					int sampled_pixels = 0;
					for (int i = -settings.noise_luminance_radius; i <= settings.noise_luminance_radius; i++) {
						for (int k = -settings.noise_luminance_radius; k <= settings.noise_luminance_radius; k++) {
							int x = pixel_x + i;
							int y = pixel_y + k;
							if (x >= 0 && x < width && y >= 0 && y < height && pixel_shape_id == shape_id.get(x, y)) {
								int dist = abs(i) + abs(k);
								avg_luminance += ycbcr.get(x, y)[0];
								sampled_pixels++;
							}
						}
					}
					avg_luminance = avg_luminance / sampled_pixels;

					// Chroma
					float avg_r = 0;
					float avg_b = 0;
					sampled_pixels = 0;
					for (int i = -settings.noise_chroma_radius; i <= settings.noise_chroma_radius; i++) {
						for (int k = -settings.noise_chroma_radius; k <= settings.noise_chroma_radius; k++) {
							int x = pixel_x + i;
							int y = pixel_y + k;
							if (x >= 0 && x < width && y >= 0 && y < height && pixel_shape_id == shape_id.get(x, y)) {
								int dist = abs(i) + abs(k);
								avg_b += ycbcr.get(x, y)[1];
								avg_r += ycbcr.get(x, y)[2];
								sampled_pixels++;
							}
						}
					}
					avg_b = avg_b / sampled_pixels;
					avg_r = avg_r / sampled_pixels;

					const color_ycbcr& pixel_ycbcr = ycbcr.get(pixel_x, pixel_y);
					float ydiff = (avg_luminance - pixel_ycbcr[0]) * settings.noise_luminance_strength;
					float bdiff = (avg_b - pixel_ycbcr[1]) * settings.noise_chroma_strength;
					float rdiff = (avg_r - pixel_ycbcr[2]) * settings.noise_chroma_strength;

					matrix ycbcr_diff = matrix(1, 3);
					ycbcr_diff.set(0, 0, ydiff);
					ycbcr_diff.set(0, 1, bdiff);
					ycbcr_diff.set(0, 2, rdiff);

					c += ycbcr_diff.vector3();
				}
			}

			// Blur
			if (settings.blur_enabled) {
				color blur_color = color();
				int sampledPixels = 0;
				for (int i = -settings.blur_radius; i <= settings.blur_radius; i++) {
					for (int j = -settings.blur_radius; j <= settings.blur_radius; j++) {
						int x = pixel_x + i;
						int y = pixel_y + j;
						if (i != 0 && j != 0 && x >= 0 && x < width && y >= 0 && y < height) {
							blur_color += ycbcr.get(x, y);
							sampledPixels++;
						}
					}
				}
				blur_color /= sampledPixels;
				c = color::lerp(c, blur_color, settings.blur_strength);
			}

			// Brightness
			if (settings.brightness_enabled) {
				c[0] += settings.brightness_adjust;
			}

			// Convert back and clamp rgb values
			c = (color_matrix_inverse.matrix_multiply(c)).vector3();
			c *= COLOR_MAX;
			float r = max(0, min(255, c.r()));
			float g = max(0, min(255, c.g()));
			float b = max(0, min(255, c.b()));
			image.set_pixel(pixel_x, pixel_y, color(r, g, b));
		}
	}
}
