//
// Created by henne on 27.11.16.
//

#pragma once

#include "GameScreen.h"

class EmptyScreen : public GameScreen {
public:
    EmptyScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);
};
