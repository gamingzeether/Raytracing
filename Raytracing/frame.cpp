#include "frame.h"

color frame::get_pixel(int x, int y) const {
	return bitmap.get(x, y);
}

void frame::set_pixel(int x, int y, color c) {
	bitmap.set(x, y, c);
}

void frame::write_to_file(std::string path) const {
	remove(path.c_str());
	std::ofstream file_stream = std::ofstream(path);

	file_stream << "P3\n" << bitmap.width() << "\n" << bitmap.height() << "\n" << COLOR_MAX;

	for (int y = bitmap.height() - 1; y >= 0; y--) {
		for (int x = 0; x < bitmap.width(); x++) {
			color pixel = get_pixel(x, y);
			file_stream << "\n" << int(pixel.r()) << " " << int(pixel.g()) << " " << int(pixel.b());
		}
	}

	file_stream.close();
}

void frame::write_to_file_bmp(std::string path) const {
	color* image_data = bitmap.data();
	bmpwriter::bmp_write(path, width(), height(), image_data);
}
