#include "GameMenu.h"

void addMenuItem(MenuItem *item, std::string text) {
	*item = {};

	for (unsigned i = 0; i < text.size(); i++) {
		item->text[i] = text[i];
	}
}

Menu *getMenuByType(GameState *gameState, MenuType type)
{
    if ((int)type >= gameState->menuCount)
    {
        return nullptr;
    }

    return &gameState->menus[(int)type];
}

void loadMainMenu(GameState *gameState) {
	gameState->player.speed = 0;

    Menu *menu = getMenuByType(gameState, MenuType::MAIN);
	*menu = {};
    menu->position = Math::Vector2f({ 20, 50 });
    menu->isVisible = true;

	addMenuItem(&menu->items[menu->numberMenuItems++], "Start Game");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Multiplayer");
	addMenuItem(&menu->items[menu->numberMenuItems++], "  Car");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Difficulty"); // empty slot for difficulty
    addMenuItem(&menu->items[menu->numberMenuItems++], "Game mode");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Credits");
	addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");
}

void loadDifficultyMenu(GameState *gameState)
{
    Menu *menu = getMenuByType(gameState, MenuType::DIFFICULTY);
    *menu = {};
    menu->position = Math::Vector2f({ 320, 50 });

    addMenuItem(&menu->items[menu->numberMenuItems++], "Easy");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Normal");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Hard");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Extreme");
}

void loadPauseMenu(GameState *gameState) {
    Menu *menu = getMenuByType(gameState, MenuType::PAUSE);
	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++],
			"Resume");

	offsetY += menu->lineSpacing;

	addMenuItem(&menu->items[menu->numberMenuItems++],
			"Main Menu");

	offsetY += menu->lineSpacing;

	addMenuItem(&menu->items[menu->numberMenuItems++],
			"Quit");
}

void loadCreditsMenu(GameState *gameState) {
    Menu *menu = getMenuByType(gameState, MenuType::CREDITS);

	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], 
			"Back");
}

/**
 * Dummy menu state for when the game is actually running
 */
void loadGameMenu(GameState *gameState) {
	//gameState->mainMenu = {};
	gameState->player.speed = PLAYER_SPEED;
}

void loadGameOverMenu(Menu *menu) {
	*menu = {};
	menu->currentMenuItem = 0;

	float offsetX = 50;
	float offsetY = 100;
	addMenuItem(&menu->items[menu->numberMenuItems++], 
			"Submit Score");

	offsetY += menu->lineSpacing;
	addMenuItem(&menu->items[menu->numberMenuItems++], 
			"Back");
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
	//loadGameOverMenu(&gameState->mainMenu);
} 

void handleMenuEnterMain(GameMemory *memory) {
    GameState *gameState = getGameState(memory);

	switch (getMenuByType(gameState, MenuType::MAIN)->currentMenuItem) {
	case 0: //Start Game
        getMenuByType(gameState, MenuType::MAIN)->isVisible = false;
        gameState->menuState = MenuState::GAME;
		break;
	case 1: //Multiplayer
		// TODO change car selection
		break;
	case 2: //Car
		// TODO change difficulty
		break;
	case 3: //Difficulty
        getMenuByType(gameState, MenuType::DIFFICULTY)->isVisible = true;
        gameState->activeMenu = MenuType::DIFFICULTY;
		break;
	case 4: //Game mode
		
		break;
    case 5: //Creadits
        gameState->menuState = MenuState::CREDITS;
        getMenuByType(gameState, MenuType::CREDITS)->isVisible = true;
        getMenuByType(gameState, MenuType::MAIN)->isVisible = false;
        gameState->activeMenu = MenuType::CREDITS;
        break;
    case 6: //Quit
        memory->exitGame();
        break;
	}
} 

//TODO: Implement method;
void handleMenuEnterDifficulty()
{
    return;
}

void handleMenuEnterPause(GameMemory *memory) {
    GameState *gameState = getGameState(memory);

    getMenuByType(gameState, MenuType::PAUSE)->isVisible = false;

	switch (getMenuByType(gameState, MenuType::PAUSE)->currentMenuItem) {
	case 0: //Resume
        gameState->menuState = MenuState::GAME;
		break;
	case 1: //Main Menu
		resetGameState(gameState);
        gameState->activeMenu = MenuType::MAIN;
        getMenuByType(gameState, MenuType::MAIN)->isVisible = true;
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
 * Process escape key being pressed
 */
void handleMenuEscape(GameMemory *memory, GameState *gameState) {
	switch (gameState->menuState) {
    case  MenuState::PAUSE:

        break;
    case MenuState::GAME:
        gameState->menuState = MenuState::PAUSE;
        getMenuByType(gameState, MenuType::PAUSE)->isVisible = true;
        gameState->activeMenu = MenuType::PAUSE;
		break;
	case  MenuState::GAME_OVER:
	case  MenuState::CREDITS:
        gameState->menuState = MenuState::MAIN;
        getMenuByType(gameState, MenuType::MAIN)->isVisible = true;
        getMenuByType(gameState, MenuType::CREDITS)->isVisible = false;
        gameState->activeMenu = MenuType::MAIN;
		break;
	case  MenuState::MAIN:
		break;
	} 
} 

/**
 * Renders a single menu item to screen
 */
void renderMenuItem(GameMemory *memory, VideoBuffer *buffer, MenuItem item, Math::Vector2f position,
		bool highlight) {
	//Math::Vector2f position = item.position;
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
	if (menu->currentMenuItem
			>= menu->numberMenuItems) {
        menu->currentMenuItem = 0;
	} else if (menu->currentMenuItem < 0) {
        menu->currentMenuItem =
            menu->numberMenuItems - 1;
	}
}

/*void changeSettingsSelection(GameState *gameState, int direction) {
	if (gameState->mainMenu.currentMenuItem == 1) { // Car setting
		gameState->player.speed = PLAYER_SPEED;
		gameState->player.nextCarIndex += direction;
		if (gameState->player.nextCarIndex < 0) {
			gameState->player.nextCarIndex = NUM_PLAYER_TEXTURES - 1;
		} else if (gameState->player.nextCarIndex >= NUM_PLAYER_TEXTURES) {
			gameState->player.nextCarIndex = 0;
		}
	} else if (gameState->mainMenu.currentMenuItem == 2) { // Difficulty setting
		gameState->difficulty += direction;
		if (gameState->difficulty < 0) {
			gameState->difficulty = NUM_DIFFICULTIES - 1;
		} else if (gameState->difficulty >= NUM_DIFFICULTIES) {
			gameState->difficulty = 0;
		}
	}
} */

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

void renderMenuBackdrop(VideoBuffer *buffer, Menu *menu) {
	Math::Rectangle rect = { };
	rect.position = menu->position;
	rect.width = WINDOW_WIDTH / 2 - 40;
	rect.height = menu->numberMenuItems
			* menu->lineSpacing + 10;
	Render::rectangle(buffer, rect, 0x80202020);
}

void updateMenu(GameMemory *memory, Input *input, Menu *menu)
{
    if (input->upKeyClicked) {
        changeMenuItemSelection(menu, -1);
    }

    if (input->downKeyClicked) {
        changeMenuItemSelection(menu, 1);
    }
}

void renderMenu(GameMemory *memory, VideoBuffer *buffer, Menu *menu)
{
    if (!menu->isVisible)
    {
        return;
    }

    renderMenuBackdrop(buffer, menu);

    // Menu items
    Math::Vector2f currentPosition = menu->position + Math::Vector2f({ 20, 50 });

    for (unsigned i = 0; i < menu->numberMenuItems; i++) {
        bool highlight = i == (unsigned)menu->currentMenuItem;
        renderMenuItem(memory, buffer, menu->items[i], currentPosition,
            highlight);

        currentPosition = currentPosition + Math::Vector2f({ 0, (float)menu->lineSpacing });
    }
}

void updateAndRenderMenu(GameMemory *memory, VideoBuffer *buffer, Input *input,
		Menu *menu) {
    GameState *gameState = getGameState(memory);

    /*
	if (gameState->mainMenu.state == MAIN) {
		if (input->leftKeyClicked) {
			changeSettingsSelection(gameState, -1);
		}
		if (input->rightKeyClicked) {
			changeSettingsSelection(gameState, 1);
		}

		bool highlightCarSelect = 1
				== (unsigned) gameState->mainMenu.currentMenuItem;
		Math::Vector2f carSelPos =
				gameState->mainMenu.items[gameState->mainMenu.currentMenuItem].position;
		renderCarSelection(memory, buffer, carSelPos, highlightCarSelect); 

		// update the car position and the speed
		gameState->player.position = gameState->player.position
				+ Math::Vector2f({ 0, (float) gameState->player.speed });
		if (gameState->player.position.y
				>= WINDOW_HEIGHT + getPlayerDimensions(gameState).y / 2) {
			gameState->player.speed = -PLAYER_SPEED;
			gameState->player.carIndex = gameState->player.nextCarIndex;
		}
		if (gameState->player.position.y <= 600) {
			gameState->player.speed = 0;
		}
	} */

	
}

/**
* Updates and renders all the menus
*/
void updateAndRenderMenus(GameMemory *memory, VideoBuffer *buffer, Input *input)
{
    GameState *gameState = getGameState(memory);

    //Update
    updateMenu(memory, input, &gameState->menus[(int)gameState->activeMenu]);

    if (input->enterKeyClicked)
    {
        switch (gameState->activeMenu)
        {
            case MenuType::MAIN:
            {
                handleMenuEnterMain(memory);
            } break;

            case MenuType::DIFFICULTY:
            {
                handleMenuEnterDifficulty();
            } break;

            case MenuType::PAUSE:
            {
                handleMenuEnterPause(memory);
            } break;
        }
    }

    if (input->escapeKeyClicked)
    {
        switch (gameState->activeMenu)
        {
            case MenuType::MAIN:
            {
            } break;

            case MenuType::DIFFICULTY:
            {
                gameState->activeMenu = MenuType::MAIN;
                getMenuByType(gameState, MenuType::DIFFICULTY)->isVisible = false;
            } break;
        }
    }
   
    //Render
    for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount; ++menuIndex)
    {
        renderMenu(memory, buffer, &gameState->menus[menuIndex]);
    }
}
