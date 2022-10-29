#include "MyMath.h"

namespace Math {

glm::vec2 from_angle(float angle) { return {cos(angle), sin(angle)}; }

float dotProduct(const glm::vec2 &v1, const glm::vec2 &v2) { return v1.x * v2.x + v1.y * v2.y; }

float length(const glm::vec2 &v) { return sqrtf(dotProduct(v, v)); }

/**
 * Angle goes from -pi to pi
 */
double angle(const glm::vec2 &v) { return atan2(v.y, v.x); }

glm::vec2 rotate(const glm::vec2 &v, double angle) {
    float x = v.x * cos(angle) - v.y * sin(angle);
    float y = v.x * sin(angle) + v.y * cos(angle);
    return glm::vec2(x, y);
}

} // namespace Math
