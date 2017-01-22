//
// Created by henne on 02.12.16.
//

#pragma once


#include "GameScreen.h"

class GameObjectScreen : public GameScreen {
public:
    GameObjectScreen(Framework &framework);

    void render(sf::RenderWindow &renderWindow);

    void update(float frameTime);

    void handleEvent(sf::Event &event);

};


