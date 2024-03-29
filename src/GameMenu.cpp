#include "GameMenu.h"

#include "Helper.h"
#include "Init.h"
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
    menu->position = glm::vec2({-1.0F, 0.8F});

    addMenuItem(&menu->items[menu->numberMenuItems++], "Submit Score");
    addMenuItem(&menu->items[menu->numberMenuItems++], "Back to Main Menu");
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
void gameOver(Platform &platform) { loadMenu(platform, MenuState::GAME_OVER); }

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
    auto color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    if (!menuHighlight) {
        color = glm::vec4(0.5, 0.5, 0.5, 1.0);
    }
    if (highlight && menuHighlight) {
        // make the highlighted item 'bounce'
        auto x = 0.0;
        if (item->bouncy) {
            item->animationTimerMs += platform.frameTimeMs;
            if (item->animationTimerMs <= 500.0) {
                x = bounce(item->animationTimerMs);
            }
        }
        position = position + (glm::vec2{x + 0.05, 0});
    }
    Render::pushText(gameState, std::string(item->text), position, Render::FontSize::Large, color, AtomPlane::MENU);
}

void updateAndRenderCarSelection(Platform &platform, GameState *gameState) {
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
    gameState->menus[2].items[direction < 0 ? 0 : 1].animationTimerMs = 500.0;
}

void menuItemSelectedPauseMenu(Platform &platform, GameState *gameState) {
    auto &activeMenu = gameState->menus[gameState->activeMenuIdx];
    if ((PauseMenuItem)activeMenu.currentMenuItemIdx == PauseMenuItem::RESUME) {
        loadMenu(platform, MenuState::GAME);
    }
    if ((PauseMenuItem)activeMenu.currentMenuItemIdx == PauseMenuItem::MAIN_MENU) {
        resetGameState(gameState);
        loadMenu(platform, MenuState::MAIN);
    }
}

void menuItemSelectedGameOverMenu(Platform &platform, GameState *gameState) {
    auto &activeMenu = gameState->menus[gameState->activeMenuIdx];
    if ((GameOverMenuItem)activeMenu.currentMenuItemIdx == GameOverMenuItem::SUBMIT_SCORE) {
        // TODO implement this
        // loadMenu(platform, MenuState::GAME);
    }
    if ((GameOverMenuItem)activeMenu.currentMenuItemIdx == GameOverMenuItem::MAIN_MENU) {
        resetGameState(gameState);
        loadMenu(platform, MenuState::MAIN);
    }
}

void menuItemSelectedMainMenu(Platform &platform, GameState *gameState) {
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
        menuItemSelectedMainMenu(platform, gameState);
    }

    if (platform.input.leftKeyClicked && gameState->activeMenuIdx == 2) {
        changeCarSelection(gameState, -1);
    }
    if (platform.input.rightKeyClicked && gameState->activeMenuIdx == 2) {
        changeCarSelection(gameState, 1);
    }

    if (gameState->activeMenuIdx == 2) {
        {
            Math::Rectangle rect = {};
            rect.position = glm::vec2(-0.3, -0.9);
            rect.size = glm::vec2(0.1, 0.2);
            if (Collision::rectangle(rect, platform.input.mousePosition) && platform.input.shootKeyClicked) {
                changeCarSelection(gameState, -1);
            }
        }

        {
            Math::Rectangle rect = {};
            rect.position = glm::vec2(0.2, -0.9);
            rect.size = glm::vec2(0.1, 0.2);
            if (Collision::rectangle(rect, platform.input.mousePosition) && platform.input.shootKeyClicked) {
                changeCarSelection(gameState, 1);
            }
        }
    }
}

void updatePauseMenu(Platform &platform) {
    GameState *gameState = getGameState(platform);

    if (platform.input.escapeKeyClicked) {
        loadMenu(platform, MenuState::GAME);
    }

    if (platform.input.enterKeyClicked) {
        menuItemSelectedPauseMenu(platform, gameState);
    }
}

void updateGameOverMenu(Platform &platform) {
    GameState *gameState = getGameState(platform);

    if (platform.input.enterKeyClicked) {
        menuItemSelectedGameOverMenu(platform, gameState);
    }
}

void updateMouseHoverOfActiveMenu(Platform &platform) {
    GameState *gameState = getGameState(platform);
    for (unsigned int menuIndex = 0; menuIndex < gameState->menuCount; ++menuIndex) {
        Menu *menu = &gameState->menus[menuIndex];
        if (!menu->isVisible) {
            continue;
        }

        bool menuHighlight = menuIndex == gameState->activeMenuIdx;
        if (!menuHighlight) {
            continue;
        }

        glm::vec2 currentPosition = menu->position - glm::vec2(-0.1, 0.2);
        for (int i = 0; i < menu->numberMenuItems; i++) {
            bool highlight = i == (unsigned)menu->currentMenuItemIdx;

            auto &item = menu->items[i];
            auto textSize = calculateTextSize(gameState, std::string(item.text), Render::FontSize::Large);
            Math::Rectangle rect = {};
            rect.position = currentPosition + glm::vec2(0, textSize.y);
            rect.size = textSize;
            if (highlight) {
                rect.size.x += 0.05;
            }

            if (Collision::rectangle(rect, platform.input.mousePosition)) {
                menu->currentMenuItemIdx = i;
                if (!Collision::rectangle(rect, platform.input.mousePositionPrevious)) {
                    menu->items[i].animationTimerMs = 0.0;
                }
                if (platform.input.shootKeyClicked) {
                    switch (gameState->menuState) {
                    case MenuState::GAME:
                        return;
                    case MenuState::MAIN:
                        menuItemSelectedMainMenu(platform, gameState);
                        break;
                    case MenuState::GAME_OVER:
                        menuItemSelectedGameOverMenu(platform, gameState);
                        break;
                    case MenuState::PAUSE:
                        menuItemSelectedPauseMenu(platform, gameState);
                        break;
                    case MenuState::CREDITS:
                        // updateCreditsMenu(platform);
                        break;
                    }
                }
                break;
            }

            currentPosition = currentPosition - glm::vec2({0, (float)menu->lineSpacing});
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
        updateGameOverMenu(platform);
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

    updateMouseHoverOfActiveMenu(platform);
}

/**
 * Updates and renders all the menus
 */
void updateAndRenderMenus(Platform &platform) {
    GameState *gameState = getGameState(platform);

    updateActiveMenu(platform);

    renderMenuBackdrop(gameState, &gameState->menus[0]);
    if (gameState->menuState == MenuState::MAIN) {
        updateAndRenderCarSelection(platform, gameState);
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
