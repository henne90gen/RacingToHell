//
// Created by henne on 27.11.16.
//
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <GameState.h>

class Framework;

/**
 * Any screen in this game is a child of this.
 */
class GameScreen {
public:

    GameScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow) = 0;

    virtual void handleEvent(sf::Event &event) = 0;

    virtual void update(float frameTime) = 0;

protected:
    Framework &_FW;
};
