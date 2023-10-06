#pragma once

#include "vec3.h"

class matrix {
public:
	matrix(int r, int c) : rows(r), columns(c) { values = new float[r * c]; };
	matrix(vec3 vector) : rows(3), columns(1) { values = new float[3] {vector.x(), vector.y(), vector.z()}; };

	inline void set(int r, int c, float val) { values[c + r * columns] = val; };
	inline float get(int r, int c) const { return values[c + r * columns]; }
	inline vec3 vector3() const { return vec3(values[0], values[1], values[2]); };

	matrix add(const matrix& other);
	matrix scalar_multiply(float scalar);
	matrix transpose();
	const matrix matrix_multiply(const matrix& other);
	const vec3 matrix_multiply(const vec3& other);
private:
	int rows;
	int columns;
	float* values;
};

