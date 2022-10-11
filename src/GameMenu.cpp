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
    menu->position = glm::vec2({-1.0F, 0.8F});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Start Game");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Car");

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
    menu->position = glm::vec2({-1.0F, 0.8F});

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

void goFromMainToCar(GameState *gameState) {
    gameState->activeMenuIdx = 2;
    gameState->player.lastCarIndex = gameState->player.carIndex;
    gameState->menus[2].items[0].animationTimerMs = 500.0;
    gameState->menus[2].items[1].animationTimerMs = 500.0;
}

void goFromCarToMain(GameState *gameState, bool shouldUpdateCarSelection) {
    if (!shouldUpdateCarSelection) {
        gameState->player.carIndex = gameState->player.lastCarIndex;
    }

    gameState->activeMenuIdx = 0;

    Menu &mainMenu = gameState->menus[gameState->activeMenuIdx];
    mainMenu.items[mainMenu.currentMenuItemIdx].animationTimerMs = 0.0;
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

float bounce(float xIn) {
    auto PI_TIMES_2 = 2.0 * PI;
    auto x = xIn - PI_TIMES_2;
    auto a = 0.025;
    auto b = 10.0;
    auto c = 0.5;
    auto y = c * sin(a * x) * (b / x);
    if (y < 0.0) {
        y *= -1.0;
    }
    return y;
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
                y = bounce(item->animationTimerMs);
            }
        }
        position = position + (glm::vec2{y + 0.05, 0});
    }
    uint32_t color = (r << 24) + (g << 16) + (b << 8) + 255;
    Render::pushText(gameState, std::string(item->text), position, Render::FontSize::Large, color, AtomPlane::MENU);
}

void updateAndRenderCarSelection(Platform &platform, GameState *gameState, bool menuHighlight) {
    auto color = glm::vec4(1, 1, 1, 1);
    auto position = glm::vec2(0, -1.1);

    {
        // use the menu items for the selection
        auto &animationTimer = gameState->menus[2].items[0].animationTimerMs;
        animationTimer -= platform.frameTimeMs;
        if (animationTimer < 0.0) {
            animationTimer = 0.0;
        }

        float offsetX = -0.2;
        offsetX -= bounce(500 - animationTimer);
        auto point1 = glm::vec2(offsetX, position.y);
        auto point2 = glm::vec2(offsetX, position.y + 0.2);
        auto point3 = glm::vec2(offsetX - 0.1, position.y + 0.1);
        Render::pushTriangle(gameState, point1, point2, point3, color, AtomPlane::MENU);
    }

    {
        // use the menu items for the selection
        auto &animationTimer = gameState->menus[2].items[1].animationTimerMs;
        animationTimer -= platform.frameTimeMs;
        if (animationTimer < 0.0) {
            animationTimer = 0.0;
        }

        float offsetX = 0.2;
        offsetX += bounce(500 - animationTimer);
        auto point1 = glm::vec2(offsetX, position.y);
        auto point2 = glm::vec2(offsetX, position.y + 0.2);
        auto point3 = glm::vec2(offsetX + 0.1, position.y + 0.1);
        Render::pushTriangle(gameState, point1, point2, point3, color, AtomPlane::MENU);
    }
}

void renderCarStatisticBars(Platform &platform) {
    // TODO render car statistic bars
}

void renderMenuBackdrop(GameState *gameState, Menu *menu) {
    Math::Rectangle rect = {};
    rect.position = menu->position;
    rect.size = glm::vec2(2.0, 1.08);
    Render::pushRectangle(gameState, rect, glm::vec4(0, 0, 0, 0.5F), AtomPlane::MENU - 0.1f);
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
    gameState->player.carIndex += direction;
    if (gameState->player.carIndex < 0) {
        gameState->player.carIndex = NUM_PLAYER_TEXTURES - 1;
    } else if (gameState->player.carIndex >= NUM_PLAYER_TEXTURES) {
        gameState->player.carIndex = 0;
    }
}

void updateMainMenu(Platform &platform) {
    auto gameState = getGameState(platform);
    if (platform.input.escapeKeyClicked) {
        if (gameState->activeMenuIdx == 0) {
            platform.exit();
        } else if (gameState->activeMenuIdx == 1) {
            goFromDifficultyToMain(gameState, false);
        } else if (gameState->activeMenuIdx == 2) {
            goFromCarToMain(gameState, false);
        } else {
            platform.abortf("Invalid active menu: {}", gameState->activeMenuIdx);
        }
    }

    if (platform.input.enterKeyClicked) {
        Menu *activeMenu = &gameState->menus[gameState->activeMenuIdx];
        switch (gameState->activeMenuIdx) {
        case 0:
            switch ((MainMenuItem)activeMenu->currentMenuItemIdx) {
            case MainMenuItem::START_GAME:
                loadMenu(platform, MenuState::GAME);
                break;
            case MainMenuItem::CHANGE_CAR:
                goFromMainToCar(gameState);
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
            goFromCarToMain(gameState, true);
            break;
        }
    }

    if (platform.input.leftKeyClicked && gameState->activeMenuIdx == 2) {
        changeCarSelection(gameState, -1);
        gameState->menus[2].items[0].animationTimerMs = 500.0;
    }
    if (platform.input.rightKeyClicked && gameState->activeMenuIdx == 2) {
        changeCarSelection(gameState, 1);
        gameState->menus[2].items[1].animationTimerMs = 500.0;
    }
}

void updatePauseMenu(Platform &platform) {
    GameState *gameState = getGameState(platform);

    if (platform.input.escapeKeyClicked) {
        loadMenu(platform, MenuState::GAME);
    }

    if (platform.input.enterKeyClicked) {
        auto &activeMenu = gameState->menus[gameState->activeMenuIdx];
        if ((PauseMenuItem)activeMenu.currentMenuItemIdx == PauseMenuItem::RESUME) {
            loadMenu(platform, MenuState::GAME);
        }
        if ((PauseMenuItem)activeMenu.currentMenuItemIdx == PauseMenuItem::MAIN_MENU) {
            // TODO reset the game state here
            loadMenu(platform, MenuState::MAIN);
        }
    }
}

void updateActiveMenu(Platform &platform) {
    GameState *gameState = getGameState(platform);

    switch (gameState->menuState) {
    case MenuState::GAME:
        return;
    case MenuState::MAIN:
        updateMainMenu(platform);
        break;
    case MenuState::GAME_OVER:
        // updateGameOverMenu(platform);
        break;
    case MenuState::PAUSE:
        updatePauseMenu(platform);
        break;
    case MenuState::CREDITS:
        // updateCreditsMenu(platform);
        break;
    }

    Menu *activeMenu = &gameState->menus[gameState->activeMenuIdx];
    if (platform.input.upKeyClicked) {
        changeMenuItemSelection(activeMenu, -1);
    }
    if (platform.input.downKeyClicked) {
        changeMenuItemSelection(activeMenu, 1);
    }
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(Platform &platform) {
    GameState *gameState = getGameState(platform);

    updateActiveMenu(platform);

    renderMenuBackdrop(gameState, &gameState->menus[0]);
    if (gameState->menuState == MenuState::MAIN) {
        updateAndRenderCarSelection(platform, gameState, 2 == gameState->activeMenuIdx);
        if (gameState->activeMenuIdx == 2) {
            renderCarStatisticBars(platform);
        }
    }

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
