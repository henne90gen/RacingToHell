// stdafx.cpp : source file that includes just the standard includes
// RacingToHell.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool rh::file_exists(const std::string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

std::string rh::floatToString(float value, unsigned int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

void rh::centerText(sf::Text &text) {
    text.setOrigin(text.getLocalBounds().width / 2, 0);
}

bool rh::pointInRectangle(sf::FloatRect rect, sf::Vector2f pos) {
    return (pos.y > rect.top && pos.y < rect.top + rect.height && pos.x > rect.left &&
            pos.x < rect.left + rect.width);
}

float rh::getAngleFromVector(sf::Vector2f vec) {
    float angle = std::atan(vec.y / vec.x) * 180.0f / PI;
    if (vec.x < 0) {
        angle += 180;
    }
    return angle;
}

sf::Vector2f rh::normalize(sf::Vector2f vec) {
    return vec / (float) (std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2)));
}
