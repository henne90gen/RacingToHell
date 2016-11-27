//
// Created by henne on 27.11.16.
//

#ifndef RACINGTOHELL_EVENTHANDLER_H
#define RACINGTOHELL_EVENTHANDLER_H

class Framework;

class EventHandler {
public:
    EventHandler(Framework &framework);

    void handleEvents(sf::RenderWindow &renderWindow);

private:
    Framework &_FW;
};


#endif //RACINGTOHELL_EVENTHANDLER_H
