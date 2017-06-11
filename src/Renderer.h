#pragma once

#include "platform.h"
#include "MyMath.h"

struct VideoBuffer;
struct GameState;
struct Texture;
struct Input;

namespace Render {

void bar(VideoBuffer *buffer, Math::Vector2f position, uint8_t length, uint32_t color);

void rectangle(VideoBuffer * buffer, Math::Rectangle rect, uint32_t color);

void circle(VideoBuffer* buffer, Math::Vector2f pos, unsigned radius, uint32_t color);

void explosion(VideoBuffer *buffer, GameState *gameState, int x, int y, unsigned *explosionIndex);

void debugInformation(VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);
void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY);

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY);

void clearScreen(VideoBuffer *buffer, int color);

}
