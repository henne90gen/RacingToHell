#include "GameMenu.h"

void addMenuItem(MenuItem *item, std::string text) {
	*item = {};

	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

void loadMainMenu(GameState *gameState) {
	Menu *menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Start Game");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Multiplayer");
	addMenuItem(&menu->items[menu->numberMenuItems++], "  Car");
	menu->items[menu->numberMenuItems - 1].bouncy = false; // remove menu bounce effect

	addMenuItem(&menu->items[menu->numberMenuItems++], "Difficulty");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Game mode");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Credits");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");

	menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 320, 50 });
	menu->isVisible = false;

	addMenuItem(&menu->items[menu->numberMenuItems++], "Easy");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Normal");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Hard");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Extreme");

	menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 320, 50 });
	menu->isVisible = false;

	addMenuItem(&menu->items[menu->numberMenuItems++], "Some");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Game");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Mode");
}

void loadPauseMenu(GameState *gameState) {
	Menu *menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Resume");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Main Menu");
}

void loadCreditsMenu(GameState *gameState) {
	Menu *menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

void loadGameOverMenu(GameState *gameState) {
	Menu *menu = &gameState->menus[gameState->menuCount++];
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Submit Score");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

void loadMenu(GameState *gameState, MenuState menuState) {
	gameState->menuState = menuState;
	gameState->menuCount = 0;
	gameState->activeMenuIndex = 0;
	for (unsigned i = 0; i < sizeof(gameState->menus) / sizeof(Menu); i++) {
		gameState->menus[i] = {};
	}

	switch (menuState) {
	case MenuState::MAIN:
		gameState->player.speed = 0;
		loadMainMenu(gameState);
		break;
	case MenuState::PAUSE:
		loadPauseMenu(gameState);
		break;
	case MenuState::GAME_OVER:
		loadGameOverMenu(gameState);
		break;
	case MenuState::CREDITS:
		loadCreditsMenu(gameState);
		break;
	case MenuState::GAME:
		gameState->player.speed = PLAYER_SPEED;
		break;
	}
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
//	loadMenu(gameState, MenuState::GAME_OVER);
}

void handleMenuEnterMain(GameMemory *memory) {
	GameState *gameState = getGameState(memory);

	Menu *activeMenu = &gameState->menus[gameState->activeMenuIndex];
	switch (gameState->activeMenuIndex) {
	case 0:
		switch (activeMenu->currentMenuItem) {
		case 0: // Start Game
			loadMenu(gameState, MenuState::GAME);
			break;
		case 1: // Multiplayer
			break;
		case 2: // Car
			break;
		case 3: // Difficulty
			gameState->activeMenuIndex = 1;
			break;
		case 4: //Game mode
			gameState->activeMenuIndex = 2;
			break;
		case 5: // Credits
			loadMenu(gameState, MenuState::CREDITS);
			break;
		case 6: // Quit
			memory->exitGame();
			break;
		}

		break;
	case 1:
		break;
	case 2:
		break;
	}
}

void handleMenuEnterPause(GameState *gameState) {
	switch (gameState->menus[0].currentMenuItem) {
	case 0: // Resume
		gameState->menuState = MenuState::GAME;
		break;
	case 1: // Main Menu
		resetGameState(gameState);
		break;
	}
}

void handleMenuEnterGameOver(GameState *gameState) {
	switch (gameState->menus[0].currentMenuItem) {
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
	switch (gameState->menuState) {
	case MenuState::MAIN:
		handleMenuEnterMain(memory);
		break;
	case MenuState::PAUSE:
		handleMenuEnterPause(gameState);
		break;
	case MenuState::GAME_OVER:
		handleMenuEnterGameOver(gameState);
		break;
	case MenuState::CREDITS:
		switch (gameState->menus[0].currentMenuItem) {
		case 0:
			loadMenu(gameState, MenuState::MAIN);
			break;
		}
		break;
	case MenuState::GAME:
		break;
	}
}

/**
 * Renders a single menu item to screen
 */
void renderMenuItem(GameMemory *memory, VideoBuffer *buffer, MenuItem *item,
		Math::Vector2f position, bool menuHighlight, bool highlight) {
	int r = 255, g = 255, b = 255;
	if (!menuHighlight) {
		r = 128;
		g = 128;
		b = 128;
	}
	if (highlight && menuHighlight) {
		// make the highlighted item 'bounce'
		float a = 5;
		if (item->bouncy) {
			item->animationCounter++;
			if (item->animationCounter <= 25) {
				a = sin(0.4 * (item->animationCounter - 2 * PI))
						* (100 / (item->animationCounter - 2 * PI));
				if (a < 0) {
					a *= -1;
				}
			}
		}
		position = position + (Math::Vector2f { 15 + a, 0 });
	}
	Text::renderText(memory, buffer, std::string(item->text), position,
			Text::FontSize::Big, r, g, b);
}

/**
 * Changes current menu item selection by 'direction'
 */
void changeMenuItemSelection(Menu *menu, int direction) {
	menu->currentMenuItem += direction;
	if (menu->currentMenuItem >= menu->numberMenuItems) {
		menu->currentMenuItem = 0;
	} else if (menu->currentMenuItem < 0) {
		menu->currentMenuItem = menu->numberMenuItems - 1;
	}
	menu->items[menu->currentMenuItem].animationCounter = 0;
}

void changeCarSelection(GameState *gameState, int direction) {
	gameState->player.speed = PLAYER_SPEED;
	gameState->player.nextCarIndex += direction;
	if (gameState->player.nextCarIndex < 0) {
		gameState->player.nextCarIndex = NUM_PLAYER_TEXTURES - 1;
	} else if (gameState->player.nextCarIndex >= NUM_PLAYER_TEXTURES) {
		gameState->player.nextCarIndex = 0;
	}
}

void renderCarSelection(GameMemory *memory, VideoBuffer *buffer,
		Math::Vector2f position, bool menuHighlight, bool highlight) {
	uint32_t color = 0xffffffff;
	if (!menuHighlight) {
		color = 0xff808080;
	}

	float offsetX = position.x;
	float a = 5;
	if (highlight && menuHighlight) {
		offsetX += 15;
		a = sin(0.1 * getGameState(memory)->frameCounter) * 7;
		if (a < 0) {
			a *= -1;
		}
		offsetX += a;
	}

	Math::Vector2f point1 = { offsetX + 12, position.y };
	Math::Vector2f point2 = { offsetX + 12, position.y + 26 };
	Math::Vector2f point3 = { offsetX, position.y + 13 };
	Render::triangle(buffer, color, point1, point2, point3);

	offsetX += 110 - 2 * a;
	point1 = {offsetX, position.y};
	point2 = {offsetX, position.y + 26};
	point3 = {offsetX + 12, position.y + 13};
	Render::triangle(buffer, color, point1, point2, point3);
}

void renderMenuBackdrop(VideoBuffer *buffer, Menu *menu) {
	Math::Rectangle rect = { };
	rect.position = menu->position;
	rect.width = WINDOW_WIDTH - 40;
	rect.height = menu->numberMenuItems * menu->lineSpacing + 10;
	Render::rectangle(buffer, rect, 0x80202020);
}

void updateMenu(GameMemory *memory, Input *input, Menu *menu) {
	if (input->upKeyClicked) {
		changeMenuItemSelection(menu, -1);
	}

	if (input->downKeyClicked) {
		changeMenuItemSelection(menu, 1);
	}

	GameState *gameState = getGameState(memory);
	if (gameState->menuState == MenuState::MAIN
			&& gameState->activeMenuIndex == 0) {
		if (menu->currentMenuItem == 2) { // Car
			if (input->leftKeyClicked) {
				changeCarSelection(gameState, -1);
			}
			if (input->rightKeyClicked) {
				changeCarSelection(gameState, 1);
			}
		}
		if (menu->currentMenuItem == 3) { // Difficulty
			gameState->menus[1].isVisible = true;
		} else {
			gameState->menus[1].isVisible = false;
		}
		if (menu->currentMenuItem == 4) { // Gamemode
			gameState->menus[2].isVisible = true;
		} else {
			gameState->menus[2].isVisible = false;
		}

		// update the car position and the speed
		gameState->player.position = gameState->player.position
				+ Math::Vector2f( { 0, (float) gameState->player.speed });
		if (gameState->player.position.y
				>= WINDOW_HEIGHT + gameState->player.size.y / 2) {
			gameState->player.speed = -PLAYER_SPEED;
			gameState->player.carIndex = gameState->player.nextCarIndex;
		}
		if (gameState->player.position.y <= 600) {
			gameState->player.speed = 0;
		}
	}
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(GameMemory *memory, VideoBuffer *buffer,
		Input *input) {
	GameState *gameState = getGameState(memory);

	// Update
	updateMenu(memory, input, &gameState->menus[gameState->activeMenuIndex]);

	if (input->enterKeyClicked) {
		handleMenuEnter(memory, gameState);
	}

	if (input->escapeKeyClicked) {
		switch (gameState->menuState) {
		case MenuState::MAIN:
			if (gameState->activeMenuIndex == 0) {
				memory->exitGame();
			} else {
				gameState->activeMenuIndex = 0;
			}
			break;
		case MenuState::CREDITS:
			loadMenu(gameState, MenuState::MAIN);
			break;
		case MenuState::PAUSE:
			loadMenu(gameState, MenuState::GAME);
			break;
		default:
			break;
		}
	}

	// Backdrop is always rendered with the first menu
	renderMenuBackdrop(buffer, &gameState->menus[0]);

	for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount;
			++menuIndex) {
		Menu *menu = &gameState->menus[menuIndex];

		if (!menu->isVisible) {
			continue;
		}
		bool menuHighlight = menuIndex == gameState->activeMenuIndex;

		// Menu items
		Math::Vector2f currentPosition = menu->position + Math::Vector2f( { 20,
				50 });

		for (unsigned i = 0; i < menu->numberMenuItems; i++) {
			bool highlight = i == (unsigned) menu->currentMenuItem;

			renderMenuItem(memory, buffer, &menu->items[i], currentPosition,
					menuHighlight, highlight);

			currentPosition = currentPosition + Math::Vector2f( { 0,
					(float) menu->lineSpacing });
		}
	}

	if (gameState->menuState == MenuState::MAIN) {
		Math::Vector2f carSelPos = gameState->menus[0].position;
		carSelPos = carSelPos + Math::Vector2f{ 20, 133 };
		bool highlightCarSelect = 2 == gameState->menus[0].currentMenuItem;
		renderCarSelection(memory, buffer, carSelPos,
				0 == gameState->activeMenuIndex, highlightCarSelect);
	}
}
