#pragma once

#include "Platform.h"
#include "RacingToHell.h"
#include "MyMath.h"

// forward declarations
struct VideoBuffer;
struct GameState;
struct Input;

// color is always a 32 bit integer in the following form
// 0xff ff ff ff
//   AA BB GG RR
// 0x50 01 02 03
// this can be read with
// uint8_t *currentColerPointer8 = (uint8_t *) color;
// uint8_t textureR = *currentColorPointer8++;
// uint8_t textureG = *currentColorPointer8++;
// uint8_t textureB = *currentColorPointer8++;
// uint8_t textureA = *currentColorPointer8++;

namespace Render {

const char firstCharacter = ' ';
const char lastCharacter = '~';

enum FontSize {
	Small, Medium, Large
};

struct Texture {
	uint32_t width, height;
	uint8_t bytesPerPixel;
	GLuint id;
};

struct Character {
	char value;
	bool hasKerning;
	Math::Vector2f size, bearing;
	float advance;
	float kerning[lastCharacter - firstCharacter];
	Render::Texture texture;
};

struct Rectangle {
	Math::Rectangle dimensions;
	uint32_t color;
};

struct TextureRectangle {
	Texture texture;
	Math::Rectangle dimensions;
	Math::Vector2f direction;
};

struct Circle {
	Math::Vector2f position;
	float radius;
	uint32_t color;	
};

struct Text {
	Math::Vector2f position;
	char characters[50];
	FontSize fontSize;
	uint32_t color;
};

void clearScreen(uint32_t color);

void pushEnableScaling(GameState *gameState, bool enable);

}
