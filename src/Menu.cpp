void addMenuItem(MenuItem *item, Math::Vector2f position, std::string text) {
	*item = {};
	item->position = position;
	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

Menu getMainMenu() {
	Menu main = { };
	main.state = MAIN;
	main.currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&main.items[main.numberMenuItems++], { offsetX, offsetY },
			"New Game");

	offsetY += 50;

	addMenuItem(&main.items[main.numberMenuItems++], { offsetX, offsetY },
			"Options");

	offsetY += 50;

	addMenuItem(&main.items[main.numberMenuItems++], { offsetX, offsetY },
			"Credits");

	offsetY += 50;

	addMenuItem(&main.items[main.numberMenuItems++], { offsetX, offsetY },
			"Quit");

	return main;
}

Menu getPauseMenu() {
	Menu pause = { };

	pause.state = PAUSE;
	pause.currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&pause.items[pause.numberMenuItems++], { offsetX, offsetY },
			"Resume");

	offsetY += 50;

	addMenuItem(&pause.items[pause.numberMenuItems++], { offsetX, offsetY },
			"Options");

	offsetY += 50;

	addMenuItem(&pause.items[pause.numberMenuItems++], { offsetX, offsetY },
			"Credits");

	offsetY += 50;

	addMenuItem(&pause.items[pause.numberMenuItems++], { offsetX, offsetY },
			"Main Menu");

	offsetY += 50;

	addMenuItem(&pause.items[pause.numberMenuItems++], { offsetX, offsetY },
			"Quit");

	return pause;
}

Menu getOptionsMenu(bool mainMenu) {
	Menu options = { };

	if (mainMenu) {
		options.state = OPTIONS_MAIN;
	} else {
		options.state = OPTIONS_PAUSE;
	}
	options.currentMenuItem = 0;

	float offsetX = 20;
	float offsetY = 100;
	addMenuItem(&options.items[options.numberMenuItems++], { offsetX, offsetY },
			"Back");

	return options;
}

Menu getGameMenu() {
	Menu game = { };
	game.state = GAME;
	return game;
}

void handleMenuEscape(GameState *gameState) {
	switch (gameState->currentMenu.state) {
	case GAME:
	case OPTIONS_PAUSE:
		gameState->currentMenu = getPauseMenu();
		break;
	case PAUSE:
		gameState->currentMenu = getGameMenu();
		break;
	case OPTIONS_MAIN:
	case GAME_OVER:
	case CREDITS:
		gameState->currentMenu = getMainMenu();
		break;
	case MAIN:
		exitGame();
		break;
	}
}

void handleMenuEnter(GameState *gameState) {
	switch (gameState->currentMenu.state) {
	case GAME:
		// do nothing
		break;
	case MAIN:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			gameState->currentMenu = getGameMenu();
			break;
		case 1:
			gameState->currentMenu = getOptionsMenu(true);
			break;
		case 2:
			// gameState->currentMenu = getCreditsMenu();
			break;
		case 3:
			exitGame();
			break;
		}
		break;
	case PAUSE:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			gameState->currentMenu = getGameMenu();
			break;
		case 1:
			gameState->currentMenu = getOptionsMenu(false);
			break;
		case 2:
			// gameState->currentMenu = getCreditsMenu();
			break;
		case 3:
			gameState->currentMenu = getMainMenu();
			break;
		case 4:
			exitGame();
			break;
		}
		break;
	case OPTIONS_MAIN:
		switch (gameState->currentMenu.currentMenuItem) {
		case 0:
			gameState->currentMenu = getMainMenu();
			break;
		}
		break;
	case OPTIONS_PAUSE:
	case GAME_OVER:
	case CREDITS:
		break;
	}
}

void renderMenuItem(VideoBuffer *buffer, MenuItem item, bool highlight) {
	Math::Vector2f position = item.position;
	if (highlight) {
		position = position + (Math::Vector2f { 20, 0 });
	}
	Text::renderText(buffer, std::string(item.text), position, 20);
}

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

// FIXME make this non-global
static bool wasEnterKeyPressed = false;
static bool wasUpKeyPressed = false;
static bool wasDownKeyPressed = false;

void updateAndRenderMenu(VideoBuffer *buffer, Input *input,
		GameState *gameState) {

	if (input->enterKey && !wasEnterKeyPressed) {
		wasEnterKeyPressed = true;
		handleMenuEnter(gameState);
	} else if (!input->enterKey) {
		wasEnterKeyPressed = false;
	}

	if (input->upKey && !wasUpKeyPressed) {
		wasUpKeyPressed = true;
		changeMenuSelection(gameState, -1);
	} else if (!input->upKey) {
		wasUpKeyPressed = false;
	}

	if (input->downKey && !wasDownKeyPressed) {
		wasDownKeyPressed = true;
		changeMenuSelection(gameState, 1);
	} else if (!input->downKey) {
		wasDownKeyPressed = false;
	}

	Math::Rectangle rect = { };
	rect.position = {10, 60};
	rect.width = WINDOW_WIDTH / 2 - 10;
	rect.height = gameState->currentMenu.numberMenuItems * 50;
	Render::rectangle(buffer, rect, 0x80202020);

	for (unsigned i = 0; i < gameState->currentMenu.numberMenuItems; i++) {
		bool highlight = i == (unsigned) gameState->currentMenu.currentMenuItem;
		renderMenuItem(buffer, gameState->currentMenu.items[i], highlight);
	}
}
