#include "GameMenu.h"

#include "Helper.h"
#include "Renderer.h"

void addMenuItem(MenuItem *item, std::string text) {
    *item = {};

    for (unsigned i = 0; i < text.size(); i++) {
        item->text[i] = text[i];
    }
}

void loadMainMenu(GameState *gameState) {
    Menu *menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({-1.0f, 0.8f});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Start Game");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Multiplayer");
    addMenuItem(&menu->items[menu->numberMenuItems++], "  Car");
    menu->items[menu->numberMenuItems - 1].bouncy = false; // remove menu bounce effect

    addMenuItem(&menu->items[menu->numberMenuItems++], "Difficulty");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Game mode");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Credits");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");

    menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({0.1, 0.8});
    menu->isVisible = false;

    addMenuItem(&menu->items[menu->numberMenuItems++], "Easy");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Normal");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Hard");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Extreme");

    menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({0.1, 0.8});
    menu->isVisible = false;

    addMenuItem(&menu->items[menu->numberMenuItems++], "Some");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Game");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Mode");
}

void loadPauseMenu(GameState *gameState) {
    Menu *menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({20, 50});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Resume");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Main Menu");
}

void loadCreditsMenu(GameState *gameState) {
    Menu *menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({20, 50});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

void loadGameOverMenu(GameState *gameState) {
    Menu *menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({20, 50});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Submit Score");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Back");
}

void loadMenu(Platform &platform, MenuState menuState) {
    GameState *gameState = getGameState(platform);
    MenuState previousMenuState = gameState->menuState;

    gameState->menuState = menuState;
    gameState->menuCount = 0;
    gameState->activeMenuIndex = 0;
    for (auto &menu : gameState->menus) {
        menu = {};
    }

    switch (menuState) {
    case MenuState::MAIN:
        //		gameState->player.speed = 0;
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
        if (previousMenuState == MenuState::MAIN) {
            generateWorld(platform);
        }
        break;
    }
}

/**
 * Switch to the game over screen
 */
void gameOver(GameState *gameState) {
    //	loadMenu(gameState, MenuState::GAME_OVER);
}

void handleMenuEnterMain(Platform &platform) {
    GameState *gameState = getGameState(platform);

    Menu *activeMenu = &gameState->menus[gameState->activeMenuIndex];
    switch (gameState->activeMenuIndex) {
    case 0:
        switch (activeMenu->currentMenuItem) {
        case 0: // Start Game
            loadMenu(platform, MenuState::GAME);
            break;
        case 1: // Multiplayer
            break;
        case 2: // Car
            break;
        case 3: // Difficulty
            gameState->activeMenuIndex = 1;
            break;
        case 4: // Game mode
            gameState->activeMenuIndex = 2;
            break;
        case 5: // Credits
            loadMenu(platform, MenuState::CREDITS);
            break;
        case 6: // Quit
            platform.exit();
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
void handleMenuEnter(Platform &platform, GameState *gameState) {
    switch (gameState->menuState) {
    case MenuState::MAIN:
        handleMenuEnterMain(platform);
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
            loadMenu(platform, MenuState::MAIN);
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
void renderMenuItem(GameState *gameState, MenuItem *item, glm::vec2 position, bool menuHighlight, bool highlight) {
    int r = 255, g = 255, b = 255;
    if (!menuHighlight) {
        r = 128;
        g = 128;
        b = 128;
    }
    if (highlight && menuHighlight) {
        // make the highlighted item 'bounce'
        float a = 0.05;
        //		if (item->bouncy) {
        //			item->animationCounter++;
        //			if (item->animationCounter <= 25) {
        //				a = sin(0.4 * (item->animationCounter - 2 *
        // PI))
        //						* (100 / (item->animationCounter -
        // 2 * PI)); 				if (a < 0) { 					a *= -1;
        //				}
        //			}
        //		}
        position = position + (glm::vec2{a, 0});
    }
    uint32_t color = (r << 24) + (g << 16) + (b << 8) + 255;
    Render::pushText(gameState, std::string(item->text), position, Render::FontSize::Large, color, AtomPlane::MENU);
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
    //	gameState->player.speed = PLAYER_SPEED;
    gameState->player.nextCarIndex += direction;
    if (gameState->player.nextCarIndex < 0) {
        gameState->player.nextCarIndex = NUM_PLAYER_TEXTURES - 1;
    } else if (gameState->player.nextCarIndex >= NUM_PLAYER_TEXTURES) {
        gameState->player.nextCarIndex = 0;
    }
}

void renderCarSelection(GameState *gameState, glm::vec2 position, bool menuHighlight, bool highlight) {
    uint32_t color = 0xffffffff;
    if (!menuHighlight) {
        color = 0xff808080;
    }

    float offsetX = position.x;
    float a = 5;
    if (highlight && menuHighlight) {
        offsetX += 15;
        a = sin(0.1 * gameState->frameCounter) * 7;
        if (a < 0) {
            a *= -1;
        }
        offsetX += a;
    }

    glm::vec2 point1 = {offsetX + 12, position.y};
    glm::vec2 point2 = {offsetX + 12, position.y + 26};
    glm::vec2 point3 = {offsetX, position.y + 13};
    Render::pushTriangle(gameState, point1, point2, point3, color, AtomPlane::MENU);

    offsetX += 110 - 2 * a;
    point1 = {offsetX, position.y};
    point2 = {offsetX, position.y + 26};
    point3 = {offsetX + 12, position.y + 13};
    Render::pushTriangle(gameState, point1, point2, point3, color, AtomPlane::MENU);
}

void renderMenuBackdrop(GameState *gameState, Menu *menu) {
    Math::Rectangle rect = {};
    rect.position = menu->position;
    // FIXME calculate size
    rect.size = glm::vec2(2.0, 1.5);
    Render::pushRectangle(gameState, rect, 0xff00ff60, AtomPlane::MENU - 0.1f);
}

void updateMenu(Platform &platform, Menu *menu) {
    if (platform.input->upKeyClicked) {
        changeMenuItemSelection(menu, -1);
    }

    if (platform.input->downKeyClicked) {
        changeMenuItemSelection(menu, 1);
    }

    GameState *gameState = getGameState(platform);
    if (gameState->menuState == MenuState::MAIN && gameState->activeMenuIndex == 0) {
        if (menu->currentMenuItem == 2) { // Car
            if (platform.input->leftKeyClicked) {
                changeCarSelection(gameState, -1);
            }
            if (platform.input->rightKeyClicked) {
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
        // FIXME movement is calculated completely different now
        //		gameState->player.position = gameState->player.position
        //				+ glm::vec2( { 0, (float)
        // gameState->player.speed }); 		if (gameState->player.position.y
        //				>= -1.0 + gameState->player.size.y / 2)
        //{ 			gameState->player.speed = -PLAYER_SPEED; gameState->player.carIndex =
        // gameState->player.nextCarIndex;
        //		}
        //		if (gameState->player.position.y <= 600) {
        //			gameState->player.speed = 0;
        //		}
    }
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(Platform &platform) {
    GameState *gameState = getGameState(platform);

    float plane = ((float)AtomPlane::MENU) - 0.5f;
    Render::pushEnableScaling(gameState, false, plane);

    // Update
    updateMenu(platform, &gameState->menus[gameState->activeMenuIndex]);

    if (platform.input->enterKeyClicked) {
        handleMenuEnter(platform, gameState);
    }

    if (platform.input->escapeKeyClicked) {
        switch (gameState->menuState) {
        case MenuState::MAIN:
            if (gameState->activeMenuIndex == 0) {
                platform.exit();
            } else {
                gameState->activeMenuIndex = 0;
            }
            break;
        case MenuState::CREDITS:
            loadMenu(platform, MenuState::MAIN);
            break;
        case MenuState::PAUSE:
            loadMenu(platform, MenuState::GAME);
            break;
        default:
            break;
        }
    }

    // Backdrop is always rendered with the first menu
    renderMenuBackdrop(getGameState(platform), &gameState->menus[0]);

    for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount; ++menuIndex) {
        Menu *menu = &gameState->menus[menuIndex];

        if (!menu->isVisible) {
            continue;
        }
        bool menuHighlight = menuIndex == gameState->activeMenuIndex;

        // Menu items
        glm::vec2 currentPosition = menu->position - glm::vec2(-0.1, 0.2);

        for (unsigned i = 0; i < menu->numberMenuItems; i++) {
            bool highlight = i == (unsigned)menu->currentMenuItem;

            renderMenuItem(getGameState(platform), &menu->items[i], currentPosition, menuHighlight, highlight);

            currentPosition = currentPosition - glm::vec2({0, (float)menu->lineSpacing});
        }
    }

    //	if (gameState->menuState == MenuState::MAIN) {
    //		glm::vec2 carSelPos = gameState->menus[0].position;
    //		carSelPos = carSelPos + glm::vec2 { 20, 133 };
    //		bool highlightCarSelect = 2 ==
    // gameState->menus[0].currentMenuItem; 		renderCarSelection(memory, carSelPos,
    // 0 == gameState->activeMenuIndex, 				highlightCarSelect);
    //	}
}
