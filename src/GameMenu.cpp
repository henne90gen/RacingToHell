#include "GameMenu.h"

/**
 * Adds a menu item to the supplied menu
 */
void addMenuItem(MenuItem *item, Math::Vector2f position, std::string text) {
	*item = {};
	item->position = position;
	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

/**
 * Sets the main menu as the current menu
 */
void loadMainMenu(Menu *menu) {
	*menu = {};
	menu->state = MAIN;
	menu->currentMenuItem = 0;
	menu->numberMenuItems = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"New Game");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Options");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Credits");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Quit");
}

/**
 * Sets the pause menu as the current menu
 */
void loadPauseMenu(Menu *menu) {
	*menu = {};
	menu->state = PAUSE;
	menu->currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Resume");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Options");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Main Menu");

	offsetY += 50;

	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Quit");
}

void loadOptionsMenu(Menu *menu, bool mainMenu) {
	*menu = {};
	if (mainMenu) {
		menu->state = OPTIONS_MAIN;
	} else {
		menu->state = OPTIONS_PAUSE;
	}
	menu->currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Back");
}

/**
 * Sets the credits menu as the current menu
 */
void loadCreditsMenu(Menu *menu) {
	*menu = {};
	menu->state = CREDITS;

	menu->currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Back");
}

/**
 * Sets a dummy menu as the current menu for when the game is running
 */
void loadGameMenu(Menu *menu) {
	*menu = {};
	menu->state = GAME;
}

/**
 * Sets the game over menu as the current menu
 */
void loadGameOverMenu(Menu *menu) {
	*menu = {};
	menu->state = GAME_OVER;
	menu->currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Submit Score");

	offsetY += 50;
	addMenuItem(&menu->items[menu->numberMenuItems++], {offsetX, offsetY},
			"Back");
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
	loadGameOverMenu(&gameState->currentMenu);
}

/**
 * Process escape key being pressed
 */
void handleMenuEscape(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.state) {
	case GAME:
	case OPTIONS_PAUSE:
		loadPauseMenu(&gameState->currentMenu);
		break;
	case PAUSE:
		loadGameMenu(&gameState->currentMenu);
		break;
	case OPTIONS_MAIN:
	case GAME_OVER:
	case CREDITS:
		loadMainMenu(&gameState->currentMenu);
		break;
	case MAIN:
		memory->exitGame();
		break;
	}
}

void handleMenuEnterMain(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.currentMenuItem) {
	case 0:
		loadGameMenu(&gameState->currentMenu);
		break;
	case 1:
		loadOptionsMenu(&gameState->currentMenu, true);
		break;
	case 2:
		loadCreditsMenu(&gameState->currentMenu);
		break;
	case 3:
		memory->exitGame();
		break;
	}
}

void handleMenuEnterPause(GameMemory *memory, GameState *gameState) {
	switch (gameState->currentMenu.currentMenuItem) {
	case 0:
		loadGameMenu(&gameState->currentMenu);
		break;
	case 1:
		loadOptionsMenu(&gameState->currentMenu, false);
		break;
	case 2:
		resetGameState(gameState);
		break;
	case 3:
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
		loadMainMenu(&gameState->currentMenu);
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
	case OPTIONS_MAIN:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			loadMainMenu(&gameState->currentMenu);
			break;
		}
		break;
	case OPTIONS_PAUSE:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			loadPauseMenu(&gameState->currentMenu);
			break;
		}
		break;
	case GAME_OVER:
		handleMenuEnterGameOver(memory, gameState);
		break;
	case CREDITS:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			loadMainMenu(&gameState->currentMenu);
			break;
		}
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
void changeMenuSelection(GameState *gameState, int direction) {
	gameState->currentMenu.currentMenuItem += direction;
	if (gameState->currentMenu.currentMenuItem
			>= gameState->currentMenu.numberMenuItems) {
		gameState->currentMenu.currentMenuItem = 0;
	} else if (gameState->currentMenu.currentMenuItem < 0) {
		gameState->currentMenu.currentMenuItem =
				gameState->currentMenu.numberMenuItems - 1;
	}
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
		changeMenuSelection(gameState, -1);
	}

	if (input->downKeyClicked) {
		changeMenuSelection(gameState, 1);
	}

	// Menu backdrop
	Math::Rectangle rect = { };
	rect.position = gameState->currentMenu.items[0].position
			- (Math::Vector2f( { 10, 40 }));
	rect.width = WINDOW_WIDTH / 2 - 10;
	rect.height = gameState->currentMenu.numberMenuItems * 50;
	Render::rectangle(buffer, rect, 0x80202020);

	// Menu items
	for (unsigned i = 0; i < gameState->currentMenu.numberMenuItems; i++) {
		bool highlight = i == (unsigned) gameState->currentMenu.currentMenuItem;
		renderMenuItem(memory, buffer, gameState->currentMenu.items[i],
				highlight);
	}
}
