#pragma once
#include "RacingToHell.h"

void gameOver(GameState *gameState);

void handleMenuEscape(GameMemory *memory, GameState* gameState);

void updateAndRenderMenus(GameMemory *memory, VideoBuffer *buffer, Input *input);
