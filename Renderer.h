#pragma once
#include "RacingToHell.h"

namespace render {

void debugInformation(VideoBuffer *buffer, Input *input,
		GameState *gameState);

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY);
void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY);

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY);

void clearScreen(VideoBuffer *buffer, int color);

}
