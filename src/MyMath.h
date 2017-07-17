#pragma once

#include <math.h>

#define PI 3.141592654f

namespace Math {

struct Vector2f {
	float x, y;
	Vector2f() :
			x(0), y(0) {
	}
	Vector2f(float angle);
	Vector2f(float x, float y) :
			x(x), y(y) {
	}
};

struct Rectangle {
	Math::Vector2f position;
	unsigned width, height;
};

Vector2f operator+(const Vector2f &v1, const Vector2f &v2);

Vector2f operator-(const Vector2f &v1, const Vector2f &v2);

Vector2f operator*(float c, const Vector2f &v);

Vector2f operator*(const Vector2f &v, float c);

float dotProduct(const Vector2f &v1, const Vector2f &v2);

float length(const Vector2f &v);

Vector2f normalize(const Vector2f &v);

}

