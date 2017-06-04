#pragma once

#include "platform.h"

struct VideoBuffer;
struct GameState;
struct Texture;
struct Input;

namespace Render {

void circle(VideoBuffer* buffer, int x, int y, unsigned radius, uint32_t color);

void explosion(VideoBuffer *buffer, GameState *gameState, int x, int y, unsigned *explosionIndex);

void debugInformation(VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);
void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY);

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY);

void clearScreen(VideoBuffer *buffer, int color);

}
