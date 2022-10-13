#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "MyMath.h"
#include "Platform.h"

// forward declarations
struct VideoBuffer;
struct GameState;
struct Input;

enum AtomPlane {
    BACKGROUND = 0,
    AI = 1,
    AI_BULLETS = 2,
    PLAYER = 3,
    PLAYER_BULLETS = 4,
    GAME_UI = 5,
    MENU = 6,
};

namespace Render {

const char firstCharacter = ' ';
const char lastCharacter = '~';

enum FontSize { Small, Medium, Large };

struct Texture {
    uint32_t width, height;
    uint8_t bytesPerPixel;
    GLuint id;
    int xDivision = 1;
    int yDivision = 1;
};

struct Character {
    char value;
    bool hasKerning;
    glm::vec2 size, bearing;
    float advance;
    float kerning[lastCharacter - firstCharacter];
    Render::Texture texture;
};

struct Rectangle {
    Math::Rectangle dimensions;
    glm::vec4 color;
};

struct Triangle {
    glm::vec2 p1, p2, p3;
    glm::vec4 color;
};

struct TextureRectangle {
    Texture texture;
    Math::Rectangle dimensions;
    glm::vec2 direction;
    int tileIndex;
};

struct Circle {
    glm::vec2 position;
    float radius;
    uint32_t color;
};

struct Text {
    glm::vec2 position;
    char characters[50];
    FontSize fontSize;
    uint32_t color;
};

void clearScreen(uint32_t color);
void flushBuffer(Platform &platform);

void pushText(GameState *gameState, std::string text, glm::vec2 position, FontSize fontSize, uint32_t color,
              AtomPlane plane);
void pushTriangle(GameState *gameState, glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, glm::vec4 color,
                  float plane);
void pushRectangle(GameState *gameState, Math::Rectangle dimensions, glm::vec4 color, float plane);
void pushCircle(GameState *gameState, glm::vec2 position, float radius, uint32_t color, AtomPlane plane);
void pushTexture(GameState *gameState, Texture *texture, glm::vec2 position, glm::vec2 size, glm::vec2 direction,
                 int tileIndex = 0, AtomPlane plane = AtomPlane::BACKGROUND);
void pushAnimation(GameState *gameState, Texture *texture, glm::vec2 position, glm::vec2 size, unsigned *tileIndex,
                   AtomPlane plane, int timing = 1);

} // namespace Render
