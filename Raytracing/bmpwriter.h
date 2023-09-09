#pragma once

#include <vector>

#include "vec3.h"

class bmpwriter
{
public:
	static std::vector<uint8_t>* bmp_bytes(int width, int height, const color* pixelData);
private:
	static void overwrite_bytes(std::vector<uint8_t>& stream, uint8_t data, size_t offset);
	static void overwrite_bytes_be(std::vector<uint8_t>& stream, uint16_t data, size_t offset);
	static void overwrite_bytes_be(std::vector<uint8_t>& stream, uint32_t data, size_t offset);
	static void overwrite_bytes_be(std::vector<uint8_t>& stream, int32_t data, size_t offset);
	static void overwrite_bytes_le(std::vector<uint8_t>& stream, uint16_t data, size_t offset);
	static void overwrite_bytes_le(std::vector<uint8_t>& stream, uint32_t data, size_t offset);
	static void overwrite_bytes_le(std::vector<uint8_t>& stream, int32_t data, size_t offset);
};

