#pragma once

#include <cmath>
#include <glm/glm.hpp>

#define PI 3.141592654f

namespace Math
{

    struct Rectangle
    {
        glm::vec2 position, size;
        // unsigned width, height;
    };

    float dotProduct(const glm::vec2 &v1, const glm::vec2 &v2);

    float length(const glm::vec2 &v);

    double angle(const glm::vec2 &v);

    glm::vec2 rotate(const glm::vec2 &v, double angle);
}
