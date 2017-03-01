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

std::string rh::to_string(float value, unsigned int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

std::string rh::to_string(sf::Vector2f vec) {
    std::string result = "";
    result += "(";
    result += rh::to_string(vec.x);
    result += "|";
    result += rh::to_string(vec.y);
    result += ")";
    return result;
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

sf::Vector2f rh::getVectorFromAngle(float angle) {
    sf::Vector2f vec(0, 0);
    vec.x = rh::round(std::cos(angle), 3);
    vec.y = rh::round(std::sin(angle), 3);
    return normalize(vec);
}

sf::Vector2f rh::normalize(sf::Vector2f vec) {
    return vec / vectorLength(vec);
}

float rh::vectorLength(sf::Vector2f vec) {
    return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
}

float rh::round(float value, int precision) {
    double precisionFactor = std::pow(10.0, precision);
    value *= precisionFactor;
    int tmp = (int) value;
    value = (float) tmp / (float) precisionFactor;
    return value;
}

sf::Vector2f rh::rotateVector(sf::Vector2f vec, float angle) {
    vec.x = vec.x * std::cos(angle) - vec.y * std::sin(angle);
    vec.y = vec.x * std::sin(angle) + vec.y * std::cos(angle);
    return vec;
}
