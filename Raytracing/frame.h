#pragma once

#include <fstream>
#include <vector>

#include "vec3.h"
#include "pngwriter.h"

#define COLOR_MAX 255

class frame {
public:
	frame(int height, int width) : h(height), w(width), pixels(new color[height * width]) {}

	int height() const { return h; };
	int width() const { return w; };
	color get_pixel(int x, int y) const;
	void set_pixel(int x, int y, color c);
	void write_to_file(std::string path) const;
	void write_to_file_bmp(std::string path) const;
private:
	int h;
	int w;
	color* pixels;
};

