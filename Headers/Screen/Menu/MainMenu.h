#pragma once

#include "Menu.h"
#include "Screen/Menu/MenuItem/StatBox.h"

class MainMenu : public Menu {
public:
    /*
        The main menu in the game. It has buttons to start playing, view the highscores, go to options or exit.
    */
    MainMenu(Framework &framework);

    ~MainMenu() {}

    /*
        Renders the MainMenu to the specified RenderWindow
        @param window Window to draw to
    */
    void render(sf::RenderWindow &window);

    /*
        Handles events for the MainMenu
    */
    void handleEvent(sf::Event &event);

    /*
        Handles the result of the different MenuItems that are on the MainMenu
        @param result What the MenuItems returned as a MenuResult
        @return GameState New GameState the game should be in
    */
    GameState handleMenuItemResult(MenuResult result);

private:
    std::shared_ptr<StatBox> _StatBox;
};

