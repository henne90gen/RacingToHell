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

void bar(VideoBuffer *buffer, Math::Vector2f position, uint8_t length,
		uint32_t color);

void triangle(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1,
		Math::Vector2f point2, Math::Vector2f point3);

void rectangle(VideoBuffer * buffer, Math::Rectangle rect, uint32_t color);

void circle(VideoBuffer* buffer, Math::Vector2f pos, unsigned radius,
		uint32_t color);

void explosion(VideoBuffer *buffer, GameState *gameState, int x, int y,
		unsigned *explosionIndex);

void debugInformation(GameMemory *memory, VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);

void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY);

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY);

void renderText(GameMemory *memory, VideoBuffer* buffer, std::string text,
		Math::Vector2f position, FontSize fontSize, uint8_t r, uint8_t g,
		uint8_t b);

void clearScreen(VideoBuffer *buffer, uint32_t color);

void clearScreen(uint32_t color);

}
