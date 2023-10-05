#pragma once

#include <cmath>

#include "vec2.h"

class vec3 {
public:
	vec3() : e{ 0,0,0 } {}
	vec3(float x, float y, float z) : e{ x,y,z } {};
	vec3(vec2 v) : e{ v.x(),0,v.y() } {}

	inline float& operator[](int i) { return e[i]; }
	inline float operator[](int i) const { return e[i]; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline vec3 operator-(const vec3& other) const { return vec3(e[0] - other.e[0], e[1] - other.e[1], e[2] - other.e[2]); }
	inline vec3 operator+(const vec3& other) const { return vec3(e[0] + other.e[0], e[1] + other.e[1], e[2] + other.e[2]); }
	inline vec3 operator*(float scalar) const { return vec3(e[0] * scalar, e[1] * scalar, e[2] * scalar); }
	inline vec3 operator/(float scalar) const { return vec3(e[0] / scalar, e[1] / scalar, e[2] / scalar); }

	inline void operator-=(const vec3& other) {
		e[0] -= other.e[0];
		e[1] -= other.e[1];
		e[2] -= other.e[2];
	}
	inline void operator+=(const vec3& other) {
		e[0] += other.e[0];
		e[1] += other.e[1];
		e[2] += other.e[2];
	}
	inline void operator*=(float scalar) {
		e[0] *= scalar;
		e[1] *= scalar;
		e[2] *= scalar;
	}
	inline void operator/=(float scalar) {
		e[0] /= scalar;
		e[1] /= scalar;
		e[2] /= scalar;
	}

	inline float length() const { return sqrt(length_squared()); }
	inline float length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	inline static float dot(const vec3& u, const vec3& v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }
	static vec3 cross(const vec3& u, const vec3& v);
	static vec3 lerp(const vec3& v1, const vec3& v2, const float& t);
	static vec3 transform(const vec3& onto, const vec3& v);

	vec3 normalized() const;
	void normalize();
private:
	float e[3];
};

using pos3 = vec3;
using color = vec3;
using color_ycbcr = vec3;
