//
// Created by henne on 30.11.16.
//

#pragma once


#include <UserInterface/Screen/GameScreen.h>

class ExitScreen : public GameScreen {
public:
    ExitScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);

private:
    sf::Clock _FadeTimer;
    float _TotalFadeTime;

    sf::Text _GoodByeText;
    sf::RectangleShape _Background;
    sf::Font _Font;
    float _FadeSpeed;
    float _InitialScale;
};


