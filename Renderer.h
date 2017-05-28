#pragma once
#include "RacingToHell.h"

namespace render {

void debugInformation(VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);
void backgroundTexture(VideoBuffer *buffer, Texture* texture);

void texture(VideoBuffer *buffer, Texture* texture);

void clearScreen(VideoBuffer *buffer, int color);

}
