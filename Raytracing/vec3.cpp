#include "vec3.h"

/// <summary>
/// Cross product of vector u and vector v
/// </summary>
/// <param name="u">Vector u</param>
/// <param name="v">Vector v</param>
/// <returns>Cross product</returns>
vec3 vec3::cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

/// <summary>
/// Linear interpolation between values v1 and v2
/// </summary>
/// <param name="v1">First value</param>
/// <param name="v2">Second value</param>
/// <param name="t">Interpolation value</param>
/// <returns></returns>
vec3 vec3::lerp(const vec3& v1, const vec3& v2, const float& t) {
	return v1 * (1 - t) + v2 * t;
}

vec3 vec3::transform(const vec3& onto, const vec3& v) {
	vec3 onto_right = vec3::cross(onto, vec3(0, 1, 0)).normalized();
	vec3 onto_up = vec3::cross(onto_right, onto).normalized();

	return onto_right * v.x() + onto_up * v.y() + onto * v.z();
}

/// <summary>
/// Returns a new vector in the same direction as the original vector
/// </summary>
/// <returns>Direction vector</returns>
vec3 vec3::normalized() const {
	return operator/(length());
}

/// <summary>
/// Normalizes vector
/// </summary>
void vec3::normalize() {
	operator/=(length());
}
