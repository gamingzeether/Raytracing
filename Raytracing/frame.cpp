#include "frame.h"

color frame::get_pixel(int x, int y) const {
	return color(pixels[y * w + x]);
}

void frame::set_pixel(int x, int y, color c) {
	pixels[y * w + x] = c;
}

void frame::write_to_file(std::string path) const {
	std::ofstream file_stream = std::ofstream(path);

	file_stream << "P3\n" << w << "\n" << h << "\n" << COLOR_MAX;

	for (int y = h - 1; y >= 0; y--) {
		for (int x = 0; x < w; x++) {
			color pixel = get_pixel(x, y);
			file_stream << "\n" << int(pixel.r()) << " " << int(pixel.g()) << " " << int(pixel.b());
		}
	}

	file_stream.close();
}
