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

    std::string floatToString(float value, unsigned int precision);

    void centerText(sf::Text &text);

    bool pointInRectangle(sf::FloatRect rect, sf::Vector2f pos);
}
