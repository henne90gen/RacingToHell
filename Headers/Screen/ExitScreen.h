//
// Created by henne on 30.11.16.
//

#pragma once


#include <Screen/GameScreen.h>

class ExitScreen : public GameScreen {
public:
    ExitScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);
};


