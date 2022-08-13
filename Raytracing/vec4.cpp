#include "vec4.h"

/// <summary>
/// Linear interpolation between values v1 and v2
/// </summary>
/// <param name="v1">First value</param>
/// <param name="v2">Second value</param>
/// <param name="t">Interpolation value</param>
/// <returns></returns>
vec4 vec4::lerp(const vec4& v1, const vec4& v2, const float& t) {
	return v1 * (1 - t) + v2 * t;
}

/// <summary>
/// Returns a new vector in the same direction as the original vector
/// </summary>
/// <returns>Direction vector</returns>
vec4 vec4::normalized() const {
	return operator/(length());
}

/// <summary>
/// Normalizes vector
/// </summary>
void vec4::normalize() {
	operator/=(length());
}

vec4 vec4::from_axis_angle(const vec3& axis, float angle) {
	float asin = sin(angle / 2);
	return vec4(cos(angle / 2), asin * axis.x(), asin * axis.y(), asin * axis.z());
}

void vec4::to_axis_angle(OUT vec3& axis, OUT float& angle) const {
	angle = 2 * acos(w());
	float sw = sqrt(1 - w() * w());
	sw = (sw < 0.001) ? 1 : sw; // Prevent div by 0
	float ax = x() / sw;
	float ay = y() / sw;
	float az = z() / sw;
	axis = vec3(ax, ay, az);
}

vec4 vec4::conjugate() const {
	return vec4(w(), -x(), -y(), -z());
}

vec4 vec4::operator*(const vec4& other) const {
	return vec4(e[0] * other.e[0] - e[1] * other.e[1] - e[2] * other.e[2] - e[3] * other.e[3],
		e[0] * other.e[1] + e[1] * other.e[0] + e[2] * other.e[3] - e[3] * other.e[2],
		e[0] * other.e[2] - e[1] * other.e[3] + e[2] * other.e[0] + e[3] * other.e[1],
		e[0] * other.e[3] + e[1] * other.e[2] - e[2] * other.e[1] + e[3] * other.e[0]);
}

vec3 vec4::vector_part() const {
	return vec3(x(), y(), z());
}

ray vec4::rotate(const ray& r, const vec3& center) const {
	vec4 qOrigin = vec4(r.origin() - center);
	vec4 qDirection = vec4(r.direction());
	vec4 conj = conjugate();

	qOrigin = this->operator*(qOrigin) * conj;
	qDirection = this->operator*(qDirection) * conj;

	return ray(qOrigin.vector_part() + center, qDirection.vector_part());
}

vec3 vec4::rotate(const vec3& v) const {
	vec4 qVector = vec4(v);
	vec4 conj = conjugate();

	qVector = this->operator*(qVector) * conj;

	return qVector.vector_part();
}
