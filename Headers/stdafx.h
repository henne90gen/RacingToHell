// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <limits>
#include <bitset>

#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#endif

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Enums.h"

#define SCREENWIDTH 600
#define SCREENHEIGHT 800
#define PI 3.141592654f
#define MIN_FPS 60.0f
#define MAX_FPS 240.0f
#define MIN_VOLUME 0.0f
#define MAX_VOLUME 100.0f

namespace rh {
    bool file_exists(const std::string &name);

    template<typename T>
    std::string to_string(T obj) {
        return std::to_string(obj);
    }

    std::string to_string(float value, unsigned int precision);

    std::string to_string(sf::Vector2f vec);

    float round(float value, int precision);

    void centerText(sf::Text &text);

    bool pointInRectangle(sf::FloatRect rect, sf::Vector2f pos);

    /**
     * Calculates the angle a vector is at
     * @param vec Vector whom's angle is going to be calculated
     * @return float Angle in degrees
     */
    float getAngleFromVector(sf::Vector2f vec);

    sf::Vector2f getVectorFromAngle(float angle);

    sf::Vector2f rotateVector(sf::Vector2f vec, float angle);

    /**
     * Divides a vector by it's length, basically providing the normalized vector
     * @param vec Vector that is going to be normalized
     * @return sf::Vector2f Resulting normalized vector
     */
    sf::Vector2f normalize(sf::Vector2f vec);

    float vectorLength(sf::Vector2f vec);

    /**
     * Deletes a object from a vector
     * @param id Index of the GameObject that will be deleted
     */
    template<typename T>
    void deleteObject(T &arr, unsigned int &id) {
        arr.erase(arr.begin() + id);
        id--;
    }
}
