#pragma once
#include "RacingToHell.h"

void handleMenuEscape(GameMemory *memory, GameState* gameState);

void updateAndRenderMenu(GameMemory *memory, VideoBuffer *buffer, Input *input,
		GameState* gameState);
