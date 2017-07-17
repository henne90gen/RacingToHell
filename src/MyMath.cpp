#include "MyMath.h"

namespace Math {

Vector2f::Vector2f(float angle) :
		x(0), y(0) {
	x = cos(angle);
	y = sin(angle);
}

Vector2f operator+(const Vector2f &v1, const Vector2f &v2) {
	Vector2f result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
}

Vector2f operator-(const Vector2f &v1, const Vector2f &v2) {
	Vector2f result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;

	return result;
}

Vector2f operator*(float c, const Vector2f &v) {
	Vector2f result;

	result.x = c * v.x;
	result.y = c * v.y;

	return result;
}

Vector2f operator*(const Vector2f &v, float c) {
	return c * v;
}

float dotProduct(const Vector2f &v1, const Vector2f &v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float length(const Vector2f &v) {
	return sqrtf(dotProduct(v, v));
}

Vector2f normalize(const Vector2f &v) {
	return v * (1.0f / length(v));
}

/**
 * Angle goes from -pi to pi
 */
double angle(const Vector2f &v) {
	return atan2(v.y, v.x);
}

Vector2f rotate(const Vector2f &v, double angle) {
	float x = v.x * cos(angle) - v.y * sin(angle);
	float y = v.x * sin(angle) + v.y * cos(angle);
	return Vector2f(x, y);
}

}

