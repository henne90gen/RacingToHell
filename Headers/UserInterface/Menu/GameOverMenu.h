#pragma once

#include "Menu.h"

class GameOverMenu : public Menu {
public:
    /*
        After the player dies this screen is shown. It has the HighscoreList on it to add the players score
    */
    GameOverMenu(Framework &framework);

    ~GameOverMenu() {}

    /*
        Renders the GameOverMenu to the specified RenderWindow
        @param window Window to draw to
    */
    virtual void render(sf::RenderWindow &window);

    /*
        Handles events for the GameOverMenu
        @param window RenderWindow to poll the events from
        @return GameState New GameState the game should be in
    */
    virtual void handleEvent(sf::Event &event);

    /*
        Update the GameOverMenu with score and level
        @parame score Score of the player
        @param level Level the player is at
    */
    virtual void update(float frameTime);

    /*
        Loads the game over sound
    */
    void load();

    /*
        Plays the game over sound
    */
    void playSounds();

    /*
        Sets the volume for the game over sound
        @param volume New volume
    */
    void setVolume(float volume);

private:
    int _Level;
    bool _ScoreSubmitted;

    //GOT = GameOverText
    sf::Text _GOTLine1;
    sf::Text _GOTLine2;
    sf::Text _GOTLine3;

    HighscoreList _HighscoreList;

    sf::SoundBuffer _GameOverSoundBuffer;
    sf::Sound _GameOverSound;
    bool _SoundPlayed;
};

