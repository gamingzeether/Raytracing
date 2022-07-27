#pragma once

#include <cmath>

#include "vec3.h"
#include "ray.h"

#define OUT

class vec4 {
public:
	vec4() : e{ 0,0,0,0 } {}
	vec4(float w, float x, float y, float z) : e{ w,x,y,z } {};
	vec4(vec3 v3) : e{ 0,v3.x(),v3.y(),v3.z() } {};

	inline float& operator[](int i) { return e[i]; }
	inline float operator[](int i) const { return e[i]; }

	inline float w() const { return e[0]; }
	inline float x() const { return e[1]; }
	inline float y() const { return e[2]; }
	inline float z() const { return e[3]; }

	inline vec4 operator-(const vec4& other) const { return vec4(e[0] - other.e[0], e[1] - other.e[1], e[2] - other.e[2], e[3] - other.e[3]); }
	inline vec4 operator+(const vec4& other) const { return vec4(e[0] + other.e[0], e[1] + other.e[1], e[2] + other.e[2], e[3] + other.e[3]); }
	inline vec4 operator*(float scalar) const { return vec4(e[0] * scalar, e[1] * scalar, e[2] * scalar, e[3] * scalar); }
	inline vec4 operator/(float scalar) const { return vec4(e[0] / scalar, e[1] / scalar, e[2] / scalar, e[3] / scalar); }

	inline void operator-=(const vec4& other) {
		e[0] -= other.e[0];
		e[1] -= other.e[1];
		e[2] -= other.e[2];
		e[3] -= other.e[3];
	}
	inline void operator+=(const vec4& other) {
		e[0] += other.e[0];
		e[1] += other.e[1];
		e[2] += other.e[2];
		e[3] += other.e[3];
	}
	inline void operator*=(float scalar) {
		e[0] *= scalar;
		e[1] *= scalar;
		e[2] *= scalar;
		e[3] *= scalar;
	}
	inline void operator/=(float scalar) {
		e[0] /= scalar;
		e[1] /= scalar;
		e[2] /= scalar;
		e[3] /= scalar;
	}

	inline float length() const { return sqrt(length_squared()); }
	inline float length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3]; }

	static vec4 lerp(const vec4& v1, const vec4& v2, const float& t);

	vec4 normalized() const;
	void normalize();

	// Quaternion functions
	static vec4 from_axis_angle(const vec3& axis, float angle);
	void to_axis_angle(OUT vec3& axis, OUT float& angle) const;
	vec4 conjugate() const;
	vec4 operator*(const vec4& other) const;
	vec3 vector_part() const;

	ray rotate(const ray& r, const vec3& center) const;
private:
	float e[4];
};

using quaternion = vec4;
using color4 = vec4;
