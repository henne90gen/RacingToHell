#pragma once

#include <UserInterface/Screen/GameScreen.h>
#include "UserInterface/Menu/MenuItem/MenuButton.h"
#include "UserInterface/Menu/MenuItem/Slider.h"
#include "UserInterface/Menu/MenuItem/Textbox.h"
#include "UserInterface/Menu/MenuItem/ComboBox.h"
#include "UserInterface/Menu/MenuItem/HighscoreList.h"
#include "UserInterface/Menu/MenuItem/PlayerTable.h"
#include "StandardCursor.h"

class Framework;

class Menu : public GameScreen {
public:
    Menu(Framework &framework, GameState menuState);

    virtual ~Menu() {}

    virtual void update(float frameTime) {}

    template<typename T>
    bool menuItemTriggered(sf::Event &event, MenuItem<T> &menuItem) {
        if (event.type == sf::Event::MouseButtonPressed) {
            _MousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        } else if (event.type == sf::Event::MouseMoved) {
            _MousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
        }
        return menuItem.handleEvent(event, _MousePos);
    }

protected:
    sf::Text _Text;
    GameState _MenuGameState;
    sf::Vector2f _MousePos;

    bool checkMenuItemHovered(sf::RenderWindow &Window, int index);
};

