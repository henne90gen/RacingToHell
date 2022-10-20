#pragma once

#include "RacingToHell.h"

void updatePlayer(Platform &platform);
void renderPlayer(GameState *gameState);
Math::Rectangle getPlayerCollisionBox(GameState *gameState);
