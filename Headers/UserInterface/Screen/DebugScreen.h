//
// Created by henne on 22.01.17.
//

#pragma once

#include "GameScreen.h"

class DebugScreen : public GameScreen {
public:
    DebugScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);

};
