#pragma once

#include <math.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define PI 3.141592654f

namespace Math
{

    struct Rectangle
    {
        glm::vec2 position, size;
        unsigned width, height;
    };

    float dotProduct(const glm::vec2 &v1, const glm::vec2 &v2);

    float length(const glm::vec2 &v);

    glm::vec2 normalize(const glm::vec2 &v);

    double angle(const glm::vec2 &v);

    glm::vec2 rotate(const glm::vec2 &v, double angle);
}
