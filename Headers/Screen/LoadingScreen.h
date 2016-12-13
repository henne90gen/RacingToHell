#pragma once

#include "Screen/Menu/Menu.h"

class LoadingScreen : public GameScreen {
public:

    LoadingScreen(Framework &framework);

    ~LoadingScreen() {}

    /*
        Renders the LoadingScreen to the specified RenderWindow
        @param window Window to draw to
    */
    virtual void render(sf::RenderWindow &window);

    /*
        Handles events for the LoadingScreen
        @param window RenderWindow to poll the events from
        @return GameState New GameState the game should be in
    */
    virtual void handleEvent(sf::Event &event);


    virtual void update(float frameTime);

private:
    float _FadeTime; // Time it takes the loading screen to fade away after the framework is done with loading

    sf::RectangleShape _Background;
    sf::Text _Text;
    int _Alpha; // sf::Uint8 is unsigned, no negatives
};

