#pragma once

#include "Menu.h"

class GameOverMenu : public Menu {
public:
    /**
     * After the player dies this screen is shown. It has the HighscoreList on it to add the players score
     */
    GameOverMenu(Framework &framework);

    ~GameOverMenu() {}

    /**
     * Renders the GameOverMenu to the specified RenderWindow
     * @param renderWindow Window to draw to
     */
    virtual void render(sf::RenderWindow &renderWindow);

    /**
     * Handles events for the GameOverMenu
     * @param window RenderWindow to poll the events from
     * @return GameState New GameState the game should be in
     */
    virtual void handleEvent(sf::Event &event);

    /**
     * Update the GameOverMenu with score and level
     * @parame score Score of the player
     * @param level Level the player is at
     */
    virtual void update(float frameTime);

private:
    bool _ScoreSubmitted;

    // GOT = GameOverText
    sf::Text _GOTLine1;
    sf::Text _GOTLine2;
    sf::Text _GOTLine3;

    HighscoreList _HighscoreList;
    Textbox _TextBox;
    MenuButton _BackBtn, _SubmitBtn;
};

