#pragma once

#include <fstream>
#include <vector>

#include "array2d.h"
#include "vec3.h"
#include "bmpwriter.h"

#define COLOR_MAX 255

class frame {
public:
	frame(int height, int width) : bitmap{ array2d<color>(height, width) } {}

	int height() const { return bitmap.height(); };
	int width() const { return bitmap.width(); };
	color get_pixel(int x, int y) const;
	void set_pixel(int x, int y, color c);
	void write_to_file(std::string path) const;
	void write_to_file_bmp(std::string path) const;
private:
	array2d<color> bitmap;
};

