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
    addMenuItem(&menu->items[menu->numberMenuItems++], "< Car >");
    menu->items[menu->numberMenuItems - 1].bouncy = false; // remove menu bounce effect

    addMenuItem(&menu->items[menu->numberMenuItems++], "Difficulty");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Credits");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Quit");

    menu = &gameState->menus[gameState->menuCount++];
    menu->position = glm::vec2({0.1, 0.8});
    menu->isVisible = false;

    addMenuItem(&menu->items[menu->numberMenuItems++], "Easy");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Normal");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Hard");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Extreme");
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
    gameState->activeMenuIdx = 0;
    for (auto &menu : gameState->menus) {
        menu = {};
    }

    switch (menuState) {
    case MenuState::MAIN:
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

void goFromMainToDifficulty(GameState *gameState) {
    gameState->activeMenuIdx = 1;

    Menu &difficultyMenu = gameState->menus[gameState->activeMenuIdx];
    difficultyMenu.isVisible = true;
    difficultyMenu.currentMenuItemIdx = gameState->difficulty;
    difficultyMenu.items[difficultyMenu.currentMenuItemIdx].animationTimerMs = 0.0;
}

void goFromDifficultyToMain(GameState *gameState, bool shouldUpdateDifficulty) {
    Menu &difficultyMenu = gameState->menus[gameState->activeMenuIdx];
    difficultyMenu.isVisible = false;
    if (shouldUpdateDifficulty) {
        gameState->difficulty = difficultyMenu.currentMenuItemIdx;
    }

    gameState->activeMenuIdx = 0;

    Menu &mainMenu = gameState->menus[gameState->activeMenuIdx];
    mainMenu.items[mainMenu.currentMenuItemIdx].animationTimerMs = 0.0;
}

void handleMenuEnterMain(Platform &platform) {
    GameState *gameState = getGameState(platform);

    Menu *activeMenu = &gameState->menus[gameState->activeMenuIdx];
    switch (gameState->activeMenuIdx) {
    case 0:
        switch ((MainMenuItem)activeMenu->currentMenuItemIdx) {
        case MainMenuItem::START_GAME:
            loadMenu(platform, MenuState::GAME);
            break;
        case MainMenuItem::CHANGE_CAR:
            break;
        case MainMenuItem::CHANGE_DIFFICULTY:
            goFromMainToDifficulty(gameState);
            break;
        case MainMenuItem::CREDITS:
            loadMenu(platform, MenuState::CREDITS);
            break;
        case MainMenuItem::EXIT:
            platform.exit();
            break;
        }
        break;
    case 1:
        goFromDifficultyToMain(gameState, true);
        break;
    case 2:
        break;
    }
}

void handleMenuEnterPause(GameState *gameState) {
    switch ((PauseMenuItem)gameState->menus[0].currentMenuItemIdx) {
    case PauseMenuItem::RESUME:
        gameState->menuState = MenuState::GAME;
        break;
    case PauseMenuItem::MAIN_MENU:
        resetGameState(gameState);
        break;
    }
}

void handleMenuEnterGameOver(GameState *gameState) {
    switch ((GameOverMenuItem)gameState->menus[0].currentMenuItemIdx) {
    case GameOverMenuItem::SUBMIT_SCORE:
        // TODO submit score
        break;
    case GameOverMenuItem::MAIN_MENU:
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
        switch (gameState->menus[0].currentMenuItemIdx) {
        case 0:
            loadMenu(platform, MenuState::MAIN);
            break;
        }
        break;
    case MenuState::GAME:
        break;
    }
}

void handleMenuEscape(Platform &platform) {
    auto gameState = getGameState(platform);

    switch (gameState->menuState) {
    case MenuState::MAIN:
        if (gameState->activeMenuIdx == 0) {
            platform.exit();
        } else if (gameState->activeMenuIdx == 1) {
            goFromDifficultyToMain(gameState, false);
        } else {
            platform.abortf("Invalid active menu: {}", gameState->activeMenuIdx);
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

/**
 * Renders a single menu item to screen
 */
void renderMenuItem(Platform &platform, GameState *gameState, MenuItem *item, glm::vec2 position, bool menuHighlight,
                    bool highlight) {
    int r = 255, g = 255, b = 255;
    if (!menuHighlight) {
        r = 128;
        g = 128;
        b = 128;
    }
    if (highlight && menuHighlight) {
        // make the highlighted item 'bounce'
        auto y = 0.0;
        if (item->bouncy) {
            item->animationTimerMs += platform.frameTimeMs;
            if (item->animationTimerMs <= 500.0) {
                auto PI_TIMES_2 = 2.0 * PI;
                auto x = item->animationTimerMs - PI_TIMES_2;
                auto a = 0.025;
                auto b = 10.0;
                auto c = 0.5;
                y = c * sin(a * x) * (b / x);
                if (y < 0.0) {
                    y *= -1.0;
                }
            }
        }
        position = position + (glm::vec2{y + 0.05, 0});
    }
    uint32_t color = (r << 24) + (g << 16) + (b << 8) + 255;
    Render::pushText(gameState, std::string(item->text), position, Render::FontSize::Large, color, AtomPlane::MENU);
}

/**
 * Changes current menu item selection by 'direction'
 */
void changeMenuItemSelection(Menu *menu, int direction) {
    menu->currentMenuItemIdx += direction;
    if (menu->currentMenuItemIdx >= menu->numberMenuItems) {
        menu->currentMenuItemIdx = 0;
    } else if (menu->currentMenuItemIdx < 0) {
        menu->currentMenuItemIdx = menu->numberMenuItems - 1;
    }
    menu->items[menu->currentMenuItemIdx].animationTimerMs = 0.0;
}

void changeCarSelection(GameState *gameState, int direction) {
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
    rect.size = glm::vec2(2.0, 1.5);
    Render::pushRectangle(gameState, rect, glm::vec4(0,0,0,0.5F), AtomPlane::MENU - 0.1f);
}

void updateMenu(Platform &platform, Menu *menu) {
    GameState *gameState = getGameState(platform);

    if (platform.input.enterKeyClicked) {
        handleMenuEnter(platform, gameState);
    }

    if (platform.input.upKeyClicked) {
        changeMenuItemSelection(menu, -1);
    }

    if (platform.input.downKeyClicked) {
        changeMenuItemSelection(menu, 1);
    }

    if (platform.input.escapeKeyClicked) {
        handleMenuEscape(platform);
    }

    if (gameState->menuState == MenuState::MAIN && gameState->activeMenuIdx == 0) {
        if ((MainMenuItem)menu->currentMenuItemIdx == MainMenuItem::CHANGE_CAR) {
            if (platform.input.leftKeyClicked) {
                platform.log("changing car to the left");
                changeCarSelection(gameState, -1);
            }
            if (platform.input.rightKeyClicked) {
                platform.log("changing car to the right");
                changeCarSelection(gameState, 1);
            }
        }
    }
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(Platform &platform) {
    GameState *gameState = getGameState(platform);

    float plane = ((float)AtomPlane::MENU) - 0.5f;
    Render::pushEnableScaling(gameState, false, plane);

    updateMenu(platform, &gameState->menus[gameState->activeMenuIdx]);

    renderMenuBackdrop(gameState, &gameState->menus[0]);

    for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount; ++menuIndex) {
        Menu *menu = &gameState->menus[menuIndex];

        if (!menu->isVisible) {
            continue;
        }
        bool menuHighlight = menuIndex == gameState->activeMenuIdx;

        // Menu items
        glm::vec2 currentPosition = menu->position - glm::vec2(-0.1, 0.2);

        for (unsigned i = 0; i < menu->numberMenuItems; i++) {
            bool highlight = i == (unsigned)menu->currentMenuItemIdx;

            renderMenuItem(platform, gameState, &menu->items[i], currentPosition, menuHighlight, highlight);

            currentPosition = currentPosition - glm::vec2({0, (float)menu->lineSpacing});
        }
    }
}
