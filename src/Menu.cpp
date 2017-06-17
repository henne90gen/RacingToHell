#include "Menu.h"

void renderMenuItem(VideoBuffer *buffer, std::string text, Math::Vector2f position) {
	Text::renderText(buffer, text, position.x, position.y, 20);
}

void updateAndRenderGameOver(VideoBuffer *buffer, Input *input,
		GameState *gameState) {

}

void updateAndRenderOptions(VideoBuffer *buffer, Input *input,
		GameState *gameState) {

}

void updateAndRenderMain(VideoBuffer *buffer, Input *input,
		GameState *gameState) {
	updateAndRenderRoad(buffer, gameState);
	renderPlayer(buffer, gameState);

	renderMenuItem(buffer, "Main Menu", Math::Vector2f {200, 100});
}

void updateAndRenderMenu(VideoBuffer *buffer, Input *input,
		GameState *gameState) {

	switch (gameState->menu) {
	case MAIN:
		updateAndRenderMain(buffer, input, gameState);
		break;
	case OPTIONS:
		updateAndRenderOptions(buffer, input, gameState);
		break;
	case GAME_OVER:
		updateAndRenderGameOver(buffer, input, gameState);
		break;
	}

}
