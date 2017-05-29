#pragma once

#include <math.h>

namespace Math {
struct Vector2f {
	float x = 0.0f;
	float y = 0.0f;
};

Vector2f operator+(const Vector2f &v1, const Vector2f &v2);
Vector2f operator-(const Vector2f &v1, const Vector2f &v2);
Vector2f operator*(float c, const Vector2f &v);
Vector2f operator*(const Vector2f &v, float c);

float dotProduct(const Vector2f &v1, const Vector2f &v2);
float length(const Vector2f &v);
Vector2f normalize(const Vector2f &v);

}

