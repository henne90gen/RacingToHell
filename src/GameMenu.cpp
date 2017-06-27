#include "GameMenu.h"

void addMenuItem(MenuItem *item, std::string text) {
	*item = {};

	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

Menu *getMenuByType(GameState *gameState, MenuType type) {
	if ((unsigned) type >= gameState->menuCount) {
		return nullptr;
	}

	return &gameState->menus[(int) type];
}

void loadMainMenu(GameState *gameState) {
	gameState->player.speed = 0;

	Menu *menu = getMenuByType(gameState, MenuType::MAIN_MENU);
	*menu = {};
	menu->position = Math::Vector2f( { 20, 50 });
	menu->isVisible = true;

	addMenuItem(&menu->items[menu->numberMenuItems++], "Start Game");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Multiplayer");
	addMenuItem(&menu->items[menu->numberMenuItems++], "  Car");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Difficulty");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Game mode");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Credits");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");
}

void loadDifficultyMenu(GameState *gameState) {
	Menu *menu = getMenuByType(gameState, MenuType::MAIN_DIFFICULTY);
	*menu = {};
	menu->position = Math::Vector2f( { 320, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Easy");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Normal");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Hard");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Extreme");
}

void loadGamemodeMenu(GameState *gameState) {
	Menu *menu = getMenuByType(gameState, MenuType::MAIN_GAMEMODE);
	*menu = {};
	menu->position = Math::Vector2f( { 320, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Some");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Game");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Mode");
}

void loadPauseMenu(GameState *gameState) {
	Menu *menu = getMenuByType(gameState, MenuType::PAUSE_MENU);
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Resume");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Main Menu");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");
}

void loadCreditsMenu(GameState *gameState) {
	Menu *menu = getMenuByType(gameState, MenuType::CREDITS_MENU);
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

void loadGameOverMenu(GameState *gameState) {
	Menu *menu = getMenuByType(gameState, MenuType::GAME_OVER_MENU);
	menu->position = Math::Vector2f( { 20, 50 });

	addMenuItem(&menu->items[menu->numberMenuItems++], "Submit Score");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
	getMenuByType(gameState, MenuType::GAME_OVER_MENU)->isVisible = true;
	gameState->menuState = MenuState::GAME_OVER;
	gameState->activeMenu = MenuType::GAME_OVER_MENU;
}

void handleMenuEnterMain(GameMemory *memory) {
	GameState *gameState = getGameState(memory);

	switch (getMenuByType(gameState, MenuType::MAIN_MENU)->currentMenuItem) {
	case 0: // Start Game
		getMenuByType(gameState, MenuType::MAIN_MENU)->isVisible = false;
		gameState->menuState = MenuState::GAME;
		gameState->player.speed = PLAYER_SPEED;
		break;
	case 1: // Multiplayer
		break;
	case 2: // Car
		break;
	case 3: // Difficulty
		getMenuByType(gameState, MenuType::MAIN_DIFFICULTY)->isVisible = true;
		gameState->activeMenu = MenuType::MAIN_DIFFICULTY;
		break;
	case 4: //Game mode
		getMenuByType(gameState, MenuType::MAIN_GAMEMODE)->isVisible = true;
		gameState->activeMenu = MenuType::MAIN_GAMEMODE;
		break;
	case 5: // Credits
		gameState->menuState = MenuState::CREDITS;
		getMenuByType(gameState, MenuType::CREDITS_MENU)->isVisible = true;
		getMenuByType(gameState, MenuType::MAIN_MENU)->isVisible = false;
		gameState->activeMenu = MenuType::CREDITS_MENU;
		break;
	case 6: // Quit
		memory->exitGame();
		break;
	}
}

//TODO: Implement method
void handleMenuEnterDifficulty() {
	return;
}

void handleMenuEnterPause(GameMemory *memory) {
	GameState *gameState = getGameState(memory);

	getMenuByType(gameState, MenuType::PAUSE_MENU)->isVisible = false;

	switch (getMenuByType(gameState, MenuType::PAUSE_MENU)->currentMenuItem) {
	case 0: //Resume
		gameState->menuState = MenuState::GAME;
		break;
	case 1: //Main Menu
		resetGameState(gameState);
		gameState->activeMenu = MenuType::MAIN_MENU;
		getMenuByType(gameState, MenuType::MAIN_MENU)->isVisible = true;
		gameState->menuState = MenuState::MAIN;
		break;
	case 2:
		memory->exitGame();
		break;
	}
}

/*void handleMenuEnterGameOver(GameMemory *memory, GameState *gameState) {
 switch (gameState->mainMenu.currentMenuItem) {
 case 0:
 // TODO submit score
 break;
 case 1:
 resetGameState(gameState);
 break;
 }
 } */

/**
 * Process enter key being pressed
 * Action depends on the current menu and the selected menu item
 */
/*void handleMenuEnter(GameMemory *memory, GameState *gameState) {
 switch (gameState->menuState)
 {
 case MenuState::GAME:
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
 switch (gameState->mainMenu.currentMenuItem) {
 case 0:
 loadMainMenu(gameState);
 break;
 }
 break;
 }
 } */

/**
 * Renders a single menu item to screen
 */
void renderMenuItem(GameMemory *memory, VideoBuffer *buffer, MenuItem item,
		Math::Vector2f position, bool highlight) {
	if (highlight) {
		position = position + (Math::Vector2f { 20, 0 });
	}
	Text::renderText(memory, buffer, std::string(item.text), position,
			Text::FontSize::Big, 255, 255, 255);
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
		Math::Vector2f position, bool highlight) {

	float offsetX = position.x;
	if (highlight) {
		offsetX += 20;
	}

	Math::Vector2f point1 = { offsetX + 12, position.y };
	Math::Vector2f point2 = { offsetX + 12, position.y + 26 };
	Math::Vector2f point3 = { offsetX, position.y + 13 };
	Render::triangle(buffer, 0xffffffff, point1, point2, point3);

	offsetX += 100;
	point1 = {offsetX, position.y};
	point2 = {offsetX, position.y + 26};
	point3 = {offsetX + 12, position.y + 13};
	Render::triangle(buffer, 0xffffffff, point1, point2, point3);
}

void renderMenuBackdrop(VideoBuffer *buffer, Menu *menu) {
	Math::Rectangle rect = { };
	rect.position = menu->position;
	rect.width = WINDOW_WIDTH / 2 - 40;
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
	if (gameState->activeMenu == MenuType::MAIN_MENU) {
		if (menu->currentMenuItem == 2) { // Car selected
			if (input->leftKeyClicked) {
				changeCarSelection(gameState, -1);
			}
			if (input->rightKeyClicked) {
				changeCarSelection(gameState, 1);
			}
		}

		// update the car position and the speed
		gameState->player.position = gameState->player.position
				+ Math::Vector2f( { 0, (float) gameState->player.speed });
		if (gameState->player.position.y
				>= WINDOW_HEIGHT + getPlayerDimensions(gameState).y / 2) {
			gameState->player.speed = -PLAYER_SPEED;
			gameState->player.carIndex = gameState->player.nextCarIndex;
		}
		if (gameState->player.position.y <= 600) {
			gameState->player.speed = 0;
		}
	}
}

void renderMenu(GameMemory *memory, VideoBuffer *buffer, Menu *menu) {
	if (!menu->isVisible) {
		return;
	}

	renderMenuBackdrop(buffer, menu);

	// Menu items
	Math::Vector2f currentPosition = menu->position
			+ Math::Vector2f( { 20, 50 });

	for (unsigned i = 0; i < menu->numberMenuItems; i++) {
		bool highlight = i == (unsigned) menu->currentMenuItem;
		renderMenuItem(memory, buffer, menu->items[i], currentPosition,
				highlight);

		currentPosition = currentPosition
				+ Math::Vector2f( { 0, (float) menu->lineSpacing });
	}

	GameState* gameState = getGameState(memory);
	if (gameState->menuState == MenuState::MAIN) {
		int activeMenuInt = (int) (gameState->activeMenu);
		bool highlightCarSelect = 2
				== gameState->menus[activeMenuInt].currentMenuItem;
		Math::Vector2f carSelPos =
				gameState->menus[(int) MenuType::MAIN_MENU].position;
		carSelPos = carSelPos + (Math::Vector2f ) { 20, 133 };
		renderCarSelection(memory, buffer, carSelPos,
				gameState->activeMenu == MenuType::MAIN_MENU
						&& highlightCarSelect);
	}
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(GameMemory *memory, VideoBuffer *buffer,
		Input *input) {
	GameState *gameState = getGameState(memory);

	//Update
	updateMenu(memory, input, &gameState->menus[(int) gameState->activeMenu]);

	if (input->enterKeyClicked) {
		switch (gameState->activeMenu) {
		case MenuType::MAIN_MENU:
			handleMenuEnterMain(memory);
			break;
		case MenuType::MAIN_DIFFICULTY:
			handleMenuEnterDifficulty();
			break;
		case MenuType::PAUSE_MENU:
			handleMenuEnterPause(memory);
			break;
		}
	}

	if (input->escapeKeyClicked) {
			switch (gameState->activeMenu) {
			case MenuType::MAIN_MENU:
				memory->exitGame();
				break;
			case MenuType::MAIN_DIFFICULTY:
				gameState->activeMenu = MenuType::MAIN_MENU;
				getMenuByType(gameState, MenuType::MAIN_DIFFICULTY)->isVisible =
						false;
				break;
			case MenuType::MAIN_GAMEMODE:
				gameState->activeMenu = MenuType::MAIN_MENU;
				getMenuByType(gameState, MenuType::MAIN_GAMEMODE)->isVisible =
						false;
				break;
			case MenuType::CREDITS_MENU:
				gameState->activeMenu = MenuType::MAIN_MENU;
				gameState->menuState = MenuState::MAIN;
				getMenuByType(gameState, MenuType::CREDITS_MENU)->isVisible =
						false;
				getMenuByType(gameState, MenuType::MAIN_MENU)->isVisible = true;
				break;
			case MenuType::PAUSE_MENU:
				gameState->menuState = MenuState::GAME;
				getMenuByType(gameState, MenuType::PAUSE_MENU)->isVisible =
						false;
				break;

			}
	}

	// Render
	for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount;
			++menuIndex) {
		renderMenu(memory, buffer, &gameState->menus[menuIndex]);
	}
}
