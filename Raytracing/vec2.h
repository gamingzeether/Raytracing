#pragma once

#include <cmath>

class vec2
{
public:
	vec2() : e{ 0,0 } {}
	vec2(float x, float y) : e{ x,y } {};

	inline float& operator[](int i) { return e[i]; }
	inline float operator[](int i) const { return e[i]; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }

	inline vec2 operator-(const vec2& other) const { return vec2(e[0] - other.e[0], e[1] - other.e[1]); }
	inline vec2 operator+(const vec2& other) const { return vec2(e[0] + other.e[0], e[1] + other.e[1]); }
	inline vec2 operator*(float scalar) const { return vec2(e[0] * scalar, e[1] * scalar); }
	inline vec2 operator/(float scalar) const { return vec2(e[0] / scalar, e[1] / scalar); }

	inline void operator-=(const vec2& other) {
		e[0] -= other.e[0];
		e[1] -= other.e[1];
	}
	inline void operator+=(const vec2& other) {
		e[0] += other.e[0];
		e[1] += other.e[1];
	}
	inline void operator*=(float scalar) {
		e[0] *= scalar;
		e[1] *= scalar;
	}
	inline void operator/=(float scalar) {
		e[0] /= scalar;
		e[1] /= scalar;
	}

	inline float length() const { return sqrt(length_squared()); }
	inline float length_squared() const { return e[0] * e[0] + e[1] * e[1]; }

	inline static float dot(const vec2& u, const vec2& v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1]; }
	static vec2 perpendicular_vector(vec2(v1));
	static vec2 lerp(const vec2& v1, const vec2& v2, const float& t);
	static vec2 transform(const vec2& onto, const vec2& v);

	vec2 normalized() const;
	void normalize();
private:
	float e[2];
};

