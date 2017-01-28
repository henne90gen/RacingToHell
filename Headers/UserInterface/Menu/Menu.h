#pragma once

#include <UserInterface/Screen/GameScreen.h>
#include "UserInterface/Menu/MenuItem/MenuButton.h"
#include "UserInterface/Menu/MenuItem/Slider.h"
#include "UserInterface/Menu/MenuItem/Textbox.h"
#include "UserInterface/Menu/MenuItem/MenuResults.h"
#include "UserInterface/Menu/MenuItem/ComboBox.h"
#include "UserInterface/Menu/MenuItem/HighscoreList.h"
#include "UserInterface/Menu/MenuItem/PlayerTable.h"
#include "StandardCursor.h"

class Framework;

class Menu : public GameScreen {
public:
    Menu(Framework &framework, GameState menuState);

    virtual ~Menu() {}

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void update(float frameTime) {}

    MenuResult getMenuItemResult(sf::Event &event);

    // FIXME are we still supporting joysticks?
    /*
    void handleJoystick(float axis);

    void applyJoystickSelection(sf::Event &Event);
    */

protected:
    std::vector<std::shared_ptr<MenuItem>> _MenuItems;
    sf::Text _Text;
    GameState _MenuGameState;
    sf::Vector2f _MousePos;

    bool checkMenuItemHovered(sf::RenderWindow &Window, int index);

//    int _JoystickSelection;
//    sf::Clock _JoystickTimer;
//    float _JoystickDelay;
};

