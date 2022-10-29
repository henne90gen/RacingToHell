#pragma once

#include <cmath>
#include <glm/glm.hpp>

#define PI 3.141592654f

namespace Math {

struct Rectangle {
    glm::vec2 position, size;
};

double angle(const glm::vec2 &v);

glm::vec2 rotate(const glm::vec2 &v, double angle);

} // namespace Math
