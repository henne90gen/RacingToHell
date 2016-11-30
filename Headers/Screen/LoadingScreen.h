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


    virtual void update(float frameTime) {}

    // FIXME think of solution for fading with screen implementation
    /*
        @return bool True if the LoadingScreen is fading away
    */
//    bool isFadingAway() { return _IsFadingAway; }

    /*
        Starts the fading of the LoadingScreen
    */
//    void fadeAway();

    /*
        @return bool True when the LoadingScreen has faded away
    */
//    bool isDoneFading();

private:
//    bool _IsFadingAway;
//    sf::Clock _FadeTimer;
//    float _FadeTime;

    sf::RectangleShape _Background;
    sf::Text _Text;
};

