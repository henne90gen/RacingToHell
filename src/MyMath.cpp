#include "MyMath.h"

namespace Math {

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

}

