//
// Created by henne on 27.11.16.
//

#pragma once

class Framework;

class EventHandler {
public:
    EventHandler(Framework &framework);

    void handleEvents(sf::RenderWindow &renderWindow);

private:
    Framework &_FW;
};
