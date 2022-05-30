#pragma once

#include <fstream>

#include "vec3.h"

#define COLOR_MAX 255

class frame {
public:
	frame(int height, int width) : h(height), w(width), pixels(new color[height * width]) {}

	int height();
	int width();
	color get_pixel(int x, int y);
	void set_pixel(int x, int y, color c);
	void write_to_file(std::string path);
private:
	int h;
	int w;
	color* pixels;
};

