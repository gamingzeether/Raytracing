#include "pngwriter.h"

std::vector<uint8_t>* pngwriter::png_bytes(int width, int height, const color* pixelData) {
	std::vector<uint8_t> bytes = std::vector<uint8_t>();

	uint32_t offset = 0;
	// Header (14 bytes)
	bytes.resize(bytes.size() + 14, 0x00);
	// File type (2 bytes) Offset:0
	overwrite_bytes_be(bytes, (uint16_t)0x424D, offset);
	offset += 2;
	// Size (4 bytes) Offset:2
	//  Set at end
	offset += 4;
	// Reserved (2 bytes) Offset:6
	offset += 2;
	// Reserved (2 bytes) Offset:8
	offset += 2;
	// Offset (4 bytes) Offset:10
	//  Set when starting writing data
	offset += 4;

	// DIB Header (BITMAPINFOHEADER: 40 bytes)
	bytes.resize(bytes.size() + 40, 0x00);
	// Header size (4 bytes) Offset: 14
	overwrite_bytes_le(bytes, (uint32_t)40, offset);
	offset += 4;
	// Width (4 bytes) Offset: 18
	overwrite_bytes_le(bytes, width, offset);
	offset += 4;
	// Height (4 bytes) Offset: 22
	overwrite_bytes_le(bytes, height, offset);
	offset += 4;
	// Color planes (2 bytes) Offset: 26
	overwrite_bytes_le(bytes, (uint16_t)1, offset);
	offset += 2;
	// Bits per pixel (2 bytes) Offset: 28
	overwrite_bytes_le(bytes, (uint16_t)24, offset);
	offset += 2;
	// Compression method (4 bytes) Offset: 30
	overwrite_bytes_le(bytes, (uint32_t)0, offset);
	offset += 4;
	// Image size (4 bytes) Offset: 34
	overwrite_bytes_le(bytes, (uint32_t)(width * height), offset);
	offset += 4;
	// Horizontal resolution in pixels / meter; signed (4 bytes) Offset: 38
	overwrite_bytes_le(bytes, (int32_t)0, offset);
	offset += 4;
	// Vertical resolution in pixels / meter; signed (4 bytes) Offset: 42
	overwrite_bytes_le(bytes, (int32_t)0, offset);
	offset += 4;
	// Number of colors in palette (4 bytes) Offset: 46
	overwrite_bytes_le(bytes, (uint32_t)0, offset);
	offset += 4;
	// Number of important colors (4 bytes) Offset: 50
	overwrite_bytes_le(bytes, (uint32_t)0, offset);
	offset += 4;

	// Palette 

	// Image data offset
	overwrite_bytes_le(bytes, (uint32_t)offset, 10);

	// Image data
	bytes.resize(bytes.size() + width * height * 3);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			const color& pixel = pixelData[i * width + j];
			overwrite_bytes(bytes, (uint8_t)pixel.b(), offset);
			overwrite_bytes(bytes, (uint8_t)pixel.g(), offset + 1);
			overwrite_bytes(bytes, (uint8_t)pixel.r(), offset + 2);
			offset += 3;
		}
	}

	// File size
	overwrite_bytes_le(bytes, (uint32_t)bytes.size(), 2);

	return &bytes;
}

void pngwriter::overwrite_bytes(std::vector<uint8_t>& stream, uint8_t data, size_t offset) {
	stream[offset] = data;
}

void pngwriter::overwrite_bytes_be(std::vector<uint8_t>& stream, uint16_t data, size_t offset) {
	stream[offset + 1] = (uint8_t)((data & 0x00FFu) >> 0);
	stream[offset + 0] = (uint8_t)((data & 0xFF00u) >> 8);
}

void pngwriter::overwrite_bytes_be(std::vector<uint8_t>& stream, uint32_t data, size_t offset) {
	stream[offset + 3] = (uint8_t)((data & 0x000000FFu) >> 0);
	stream[offset + 2] = (uint8_t)((data & 0x0000FF00u) >> 8);
	stream[offset + 1] = (uint8_t)((data & 0x00FF0000u) >> 16);
	stream[offset + 0] = (uint8_t)((data & 0xFF000000u) >> 24);
}

void pngwriter::overwrite_bytes_be(std::vector<uint8_t>& stream, int32_t data, size_t offset) {
	stream[offset + 3] = (uint8_t)((data & 0x000000FFu) >> 0);
	stream[offset + 2] = (uint8_t)((data & 0x0000FF00u) >> 8);
	stream[offset + 1] = (uint8_t)((data & 0x00FF0000u) >> 16);
	stream[offset + 0] = (uint8_t)((data & 0xFF000000u) >> 24);
}

void pngwriter::overwrite_bytes_le(std::vector<uint8_t>& stream, uint16_t data, size_t offset) {
	stream[offset + 0] = (uint8_t)((data & 0x00FFu) >> 0);
	stream[offset + 1] = (uint8_t)((data & 0xFF00u) >> 8);
}

void pngwriter::overwrite_bytes_le(std::vector<uint8_t>& stream, uint32_t data, size_t offset) {
	stream[offset + 0] = (uint8_t)((data & 0x000000FFu) >> 0);
	stream[offset + 1] = (uint8_t)((data & 0x0000FF00u) >> 8);
	stream[offset + 2] = (uint8_t)((data & 0x00FF0000u) >> 16);
	stream[offset + 3] = (uint8_t)((data & 0xFF000000u) >> 24);
}

void pngwriter::overwrite_bytes_le(std::vector<uint8_t>& stream, int32_t data, size_t offset) {
	stream[offset + 0] = (uint8_t)((data & 0x000000FFu) >> 0);
	stream[offset + 1] = (uint8_t)((data & 0x0000FF00u) >> 8);
	stream[offset + 2] = (uint8_t)((data & 0x00FF0000u) >> 16);
	stream[offset + 3] = (uint8_t)((data & 0xFF000000u) >> 24);
}