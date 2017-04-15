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

float rh::angleFromVector(sf::Vector2f vec) {
    float angle = std::atan(vec.y / vec.x) * 180.0f / PI;
    if (vec.x < 0) {
        angle += 180;
    }
    return angle;
}

sf::Vector2f rh::vectorFromAngle(float angle) {
    sf::Vector2f vec;
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
    float x = vec.x * std::cos(angle) - vec.y * std::sin(angle);
    float y = vec.x * std::sin(angle) + vec.y * std::cos(angle);
    if (x > -0.0000001 && x <= 0) x = 0;
    if (y > -0.0000001 && y <= 0) y = 0;
    vec = sf::Vector2f(x, y);
    return vec;
}

float rh::angleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2) {
    // Using standard vector angle formula, which gives an unsigned angle
    float angle = (vec1.x * vec2.x + vec1.y * vec2.y) / (vectorLength(vec1) * vectorLength(vec2));

    // Getting second vector for transformation matrix
    sf::Vector2f rot = rotateVector(vec1, PI / 2);
    // Calculating values of the transformation matrix that looks like this:
    // (a b)
    // (c d)
    float c = 1 / (rot.x - (rot.y / vec1.y) * vec1.x);
    float d = 1 / (rot.y - (rot.x / vec1.x) * vec1.y);

    int sign = 1;
    if (c * vec2.x + d * vec2.y < 0) {
        sign = -1;
    }
    return sign * std::acos(angle);
}
