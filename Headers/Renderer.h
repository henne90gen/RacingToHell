//
// Created by henne on 27.11.16.
//

#ifndef RACINGTOHELL_RENDERER_H
#define RACINGTOHELL_RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "GameState.h"

/**
 * Forward declarations
 */
class Framework;

class Renderer {
public:

    Renderer(Framework &framework);

    void render();

    sf::RenderWindow &getRenderWindow() {
        return _RenderWindow;
    }
    void closeRenderWindow() {
        _RenderWindow.close();
    }

private:
    Framework &_FW;
    sf::RenderWindow _RenderWindow;

    void setMouseVisibility();
};

#endif //RACINGTOHELL_RENDERER_H
