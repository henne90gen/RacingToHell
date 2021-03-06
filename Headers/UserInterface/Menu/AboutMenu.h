#pragma once

#include "Menu.h"

class AboutMenu : public Menu {
public:
    AboutMenu(Framework &framework);

    ~AboutMenu() {}

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

private:
    sf::Text _Text2, _LevelUp, _LevelUp2, _Music, _Cars, _Boss, _Boss2, _AIShotSounds, _AIShotSounds2, _PlayerCarShotSounds, _PlayerCarShotSounds2;
    sf::RectangleShape _Frame;
    MenuButton _BackBtn;
};

