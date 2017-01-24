#pragma once

#include "Menu.h"

class HighscoreMenu : public Menu {
public:
    /*
        Shows the HighscoreList, nothing more
    */
    HighscoreMenu(Framework &framework);

    ~HighscoreMenu() {}

    /*
        Renders the HighscoreMenu to the specified RenderWindow
        @param window Window to draw to
    */
    virtual void render(sf::RenderWindow &Window);

    /*
        Handles events for the HighscoreMenu
        @param window RenderWindow to poll the events from
        @return GameState New GameState the game should be in
    */
    virtual void handleEvent(sf::Event &event);

    void loadScoreTable() { _List.loadScoreTable(); }

private:
    HighscoreList _List;
};

