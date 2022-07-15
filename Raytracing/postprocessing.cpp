#include "postprocessing.h"

void postprocessing::blur(frame& image, int radius) {
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
			temp.set_pixel(i, j, c);
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
