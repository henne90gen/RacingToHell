#pragma once
#include "RacingToHell.h"

void gameOver(GameState *gameState);

void handleMenuEscape(GameMemory *memory, GameState* gameState);

void updateAndRenderMenu(GameMemory *memory, VideoBuffer *buffer, Input *input,
		GameState* gameState);
