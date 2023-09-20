#include "vec2.h"

/// <summary>
/// Vector rotated 90 degrees
/// </summary>
/// <param name="v1">Vector</param>
/// <returns>Perpendicular vector</returns>
vec2 vec2::perpendicular_vector(vec2 v1) {
	return vec2(v1.y(), -v1.x());
}

/// <summary>
/// Linear interpolation between values v1 and v2
/// </summary>
/// <param name="v1">First value</param>
/// <param name="v2">Second value</param>
/// <param name="t">Interpolation value</param>
/// <returns></returns>
vec2 vec2::lerp(const vec2& v1, const vec2& v2, const float& t) {
	return v1 * (1 - t) + v2 * t;
}

vec2 vec2::transform(const vec2& onto, const vec2& v) {
	vec2 perp = vec2::perpendicular_vector(onto);

	return perp * v.x() + onto * v.y();
}

/// <summary>
/// Returns a new vector in the same direction as the original vector
/// </summary>
/// <returns>Direction vector</returns>
vec2 vec2::normalized() const {
	return operator/(length());
}

/// <summary>
/// Normalizes vector
/// </summary>
void vec2::normalize() {
	operator/=(length());
}
