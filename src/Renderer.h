#pragma once

#include "platform.h"
#include "MyMath.h"

struct VideoBuffer;
struct GameState;
struct Texture;
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

void bar(VideoBuffer *buffer, Math::Vector2f position, uint8_t length, uint32_t color);

void triangle(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1, Math::Vector2f point2, Math::Vector2f point3);

void rectangle(VideoBuffer * buffer, Math::Rectangle rect, uint32_t color);

void circle(VideoBuffer* buffer, Math::Vector2f pos, unsigned radius, uint32_t color);

void explosion(VideoBuffer *buffer, GameState *gameState, int x, int y, unsigned *explosionIndex);

void debugInformation(VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);
void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY);

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY);

void clearScreen(VideoBuffer *buffer, uint32_t color);

}
