#pragma once

#include <Screen/GameScreen.h>
#include "Screen/Menu/MenuItem/MenuButton.h"
#include "Screen/Menu/MenuItem/Slider.h"
#include "Screen/Menu/MenuItem/Textbox.h"
#include "Screen/Menu/MenuItem/MenuResults.h"
#include "Screen/Menu/MenuItem/ComboBox.h"
#include "Screen/Menu/MenuItem/HighscoreList.h"
#include "Screen/Menu/MenuItem/PlayerTable.h"
#include "StandardCursor.h"

class Framework;

class Menu : public GameScreen {
public:
    Menu(Framework &framework, GameState menuState, GameState returnState);

    ~Menu() {}

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void update(float frameTime) {}

    GameState handleMenuItems(sf::Event &event);

    // FIXME are we still supporting joysticks?
    /*
    void handleJoystick(float axis);

    void applyJoystickSelection(sf::Event &Event);
    */

protected:
    std::vector<std::shared_ptr<MenuItem>> _MenuItems;
    sf::Text _Text;
    GameState _MenuGameState;
    GameState _ReturnGameState;
    sf::Vector2f _MousePos;

    virtual GameState handleMenuItemResult(MenuResult result) = 0;

    bool checkMenuItemHovered(sf::RenderWindow &Window, int index);

//    int _JoystickSelection;
//    sf::Clock _JoystickTimer;
//    float _JoystickDelay;
};

