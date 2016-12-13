//
// Created by henne on 27.11.16.
//

#pragma once

#include "Screen/GameScreenFactory.h"
#include "Enums.h"

class Framework;

class Renderer {
public:

    Renderer(Framework &framework);

    void render();

//    GameScreen &getCurrentGameScreen() {
//        return *_DisplayedGameScreens.at(0);
//    }

    /**
     * Updating the screens that are currently being displayed, the front of the array is in the background and the
     * back of the array is in the foreground.
     */
    void updateGameScreens();

//    sf::RenderWindow &getRenderWindow() {
//        return _RenderWindow;
//    }

//    void closeRenderWindow() {
//        _RenderWindow.close();
//    }

private:
    Framework &_FW;

};
