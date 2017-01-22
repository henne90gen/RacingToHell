//
// Created by henne on 29.11.16.
//

#pragma once


#include <UserInterface/Screen/GameScreen.h>

class LevelScreen : public GameScreen {
public:
    LevelScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);
};


