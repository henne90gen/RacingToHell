#include "GameMenu.h"

void addMenuItem(MenuItem *item, Math::Vector2f position, std::string text) {
	*item = {};
	item->position = position;
	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

void loadMainMenu(GameState *gameState) {
	gameState->player.speed = 0;

	Menu *menu = &gameState->currentMenu;
	*menu = {};
	menu->state = MAIN;

	Math::Vector2f offset = { 50, 100 };

	addMenuItem(&menu->items[menu->numberMenuItems++], offset, "Start Game");

	offset.y += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], offset, "  Car");

	offset.y += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], offset, "Difficulty"); // empty slot for difficulty

	offset.y += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], offset, "Credits");

	offset.y += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], offset, "Quit");
}

void loadPauseMenu(Menu *menu) {
	*menu = {};
	menu->state = PAUSE;
	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Resume");

	offsetY += menu->lineSpacing;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Main Menu");

	offsetY += menu->lineSpacing;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Quit");
}

void loadCreditsMenu(Menu *menu) {
	*menu = {};
	menu->state = CREDITS;

	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Back");
}

/**
 * Dummy menu state for when the game is actually running
 */
void loadGameMenu(GameState *gameState) {
	gameState->currentMenu = {};
	gameState->currentMenu.state = GAME;
	gameState->player.speed = PLAYER_SPEED;
}

void loadGameOverMenu(Menu *menu) {
	*menu = {};
	menu->state = GAME_OVER;
	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Submit Score");

	offsetY += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Back");
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
	loadGameOverMenu(&gameState->currentMenu);
}

void handleMenuEnterMain(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.currentMenuItem) {
	case 0:
		loadGameMenu(gameState);
		break;
	case 1:
		// TODO change car selection
		break;
	case 2:
		// TODO change difficulty
		break;
	case 3:
		loadCreditsMenu(&gameState->currentMenu);
		break;
	case 4:
		memory->exitGame();
		break;
	}
}

void handleMenuEnterPause(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.currentMenuItem) {
	case 0:
		loadGameMenu(gameState);
		break;
	case 1:
		resetGameState(gameState);
		break;
	case 2:
		memory->exitGame();
		break;
	}
}

void handleMenuEnterGameOver(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.currentMenuItem) {
	case 0:
		// TODO submit score
		break;
	case 1:
		resetGameState(gameState);
		break;
	}
}

/**
 * Process enter key being pressed
 * Action depends on the current menu and the selected menu item
 */
void handleMenuEnter(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.state) {
	case GAME:
		// do nothing
		break;
	case MAIN:
		handleMenuEnterMain(memory, gameState);
		break;
	case PAUSE:
		handleMenuEnterPause(memory, gameState);
		break;
	case GAME_OVER:
		handleMenuEnterGameOver(memory, gameState);
		break;
	case CREDITS:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			loadMainMenu(gameState);
			break;
		}
		break;
	}
}

/**
 * Process escape key being pressed
 */
void handleMenuEscape(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.state) {
	case GAME:
		loadPauseMenu(&gameState->currentMenu);
		break;
	case PAUSE:
		loadGameMenu(gameState);
		break;
	case GAME_OVER:
	case CREDITS:
		loadMainMenu(gameState);
		break;
	case MAIN:
		memory->exitGame();
		break;
	}
}

/**
 * Renders a single menu item to screen
 */
void renderMenuItem(GameMemory *memory, VideoBuffer *buffer, MenuItem item,
		bool highlight) {
	Math::Vector2f position = item.position;
	if (highlight) {
		position = position + (Math::Vector2f { 20, 0 });
	}
	Text::renderText(memory, buffer, std::string(item.text), position,
			Text::FontSize::Big, 255, 255, 255);
}

/**
 * Changes current menu item selection by 'direction'
 */
void changeMenuItemSelection(GameState *gameState, int direction) {
	gameState->currentMenu.currentMenuItem += direction;
	if (gameState->currentMenu.currentMenuItem
			>= gameState->currentMenu.numberMenuItems) {
		gameState->currentMenu.currentMenuItem = 0;
	} else if (gameState->currentMenu.currentMenuItem < 0) {
		gameState->currentMenu.currentMenuItem =
				gameState->currentMenu.numberMenuItems - 1;
	}
}

void changeSettingsSelection(GameState *gameState, int direction) {
	if (gameState->currentMenu.currentMenuItem == 1) { // Car setting
		gameState->player.speed = PLAYER_SPEED;
		gameState->player.nextCarIndex += direction;
		if (gameState->player.nextCarIndex < 0) {
			gameState->player.nextCarIndex = NUM_PLAYER_TEXTURES - 1;
		} else if (gameState->player.nextCarIndex >= NUM_PLAYER_TEXTURES) {
			gameState->player.nextCarIndex = 0;
		}
	} else if (gameState->currentMenu.currentMenuItem == 2) { // Difficulty setting
		gameState->difficulty += direction;
		if (gameState->difficulty < 0) {
			gameState->difficulty = NUM_DIFFICULTIES - 1;
		} else if (gameState->difficulty >= NUM_DIFFICULTIES) {
			gameState->difficulty = 0;
		}
	}
}

void renderCarSelection(GameMemory *memory, VideoBuffer *buffer,
		Math::Vector2f position, bool highlight) {


	float offsetX = position.x;
	if (highlight) {
		offsetX += 20;
	}

	Math::Vector2f point1 = { offsetX + 12, 142 };
	Math::Vector2f point2 = { offsetX + 12, 168 };
	Math::Vector2f point3 = { offsetX, 155 };
	Render::triangle(buffer, 0xffffffff, point1, point2, point3);

	offsetX += 100;
	point1 = {offsetX, 141};
	point2 = {offsetX, 169};
	point3 = {offsetX + 12, 155};
	Render::triangle(buffer, 0xffffffff, point1, point2, point3);
}

void renderMenuBackdrop(VideoBuffer *buffer, GameState *gameState) {
	Math::Rectangle rect = { };
	rect.position = gameState->currentMenu.items[0].position
			- (Math::Vector2f( { 20, 50 }));
	rect.width = WINDOW_WIDTH - 60;
	rect.height = gameState->currentMenu.numberMenuItems
			* gameState->currentMenu.lineSpacing;
	Render::rectangle(buffer, rect, 0x80202020);
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenu(GameMemory *memory, VideoBuffer *buffer, Input *input,
		GameState *gameState) {

	if (input->enterKeyClicked) {
		handleMenuEnter(memory, gameState);
	}

	if (input->upKeyClicked) {
		changeMenuItemSelection(gameState, -1);
	}

	if (input->downKeyClicked) {
		changeMenuItemSelection(gameState, 1);
	}

	renderMenuBackdrop(buffer, gameState);

	if (gameState->currentMenu.state == MAIN) {
		if (input->leftKeyClicked) {
			changeSettingsSelection(gameState, -1);
		}
		if (input->rightKeyClicked) {
			changeSettingsSelection(gameState, 1);
		}

		bool highlightCarSelect = 1
				== (unsigned) gameState->currentMenu.currentMenuItem;
		Math::Vector2f carSelPos =
				gameState->currentMenu.items[gameState->currentMenu.currentMenuItem].position;
		renderCarSelection(memory, buffer, carSelPos, highlightCarSelect);

		// update the car position and the speed
		gameState->player.position = gameState->player.position
				+ (Math::Vector2f ) { 0, (float) gameState->player.speed };
		if (gameState->player.position.y
				>= WINDOW_HEIGHT + getPlayerDimensions(gameState).y / 2) {
			gameState->player.speed = -PLAYER_SPEED;
			gameState->player.carIndex = gameState->player.nextCarIndex;
		}
		if (gameState->player.position.y <= 600) {
			gameState->player.speed = 0;
		}
	}

	// Menu items
	for (unsigned i = 0; i < gameState->currentMenu.numberMenuItems; i++) {
		bool highlight = i == (unsigned) gameState->currentMenu.currentMenuItem;
		renderMenuItem(memory, buffer, gameState->currentMenu.items[i],
				highlight);
	}
}
