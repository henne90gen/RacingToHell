#pragma once

#include "Menu.h"

class PauseMenu : public Menu {
public:

    PauseMenu(Framework &framework);

    ~PauseMenu() {}

    /**
     * Renders the PauseMenu to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &renderWindow);

    /**
     * Handles events for the PauseMenu
     * @param window RenderWindow to poll the events from
     * @return GameState New GameState the game should be in
     */
    virtual void handleEvent(sf::Event &event);

private:
    MenuButton _ExitBtn, _BackToMainBtn, _OptionsBtn, _ResumeBtn;
};

