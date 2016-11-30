#include "stdafx.h"
#include "Screen/Menu/HighscoreMenu.h"
#include "Framework/Framework.h"

HighscoreMenu::HighscoreMenu(Framework &framework) : Menu(framework, GameState::Highscores),
                                                     _List(sf::Vector2f(75, 100)) {
    sf::Vector2f ButtonSize = sf::Vector2f(150, 50);
    std::shared_ptr<MenuButton> button(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 200, 735), ButtonSize, MenuResult::Back, "Back",
                           TextAlignment::Center));
    _MenuItems.push_back(button);

    _Text.setString("Highscores");
    _Text.setCharacterSize(53);
    _Text.setColor(sf::Color::White);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}

void HighscoreMenu::render(sf::RenderWindow &window) {
    _List.render(window);

    Menu::render(window);
}

void HighscoreMenu::handleEvent(sf::Event &event) {

//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

//    if (event.type == sf::Event::JoystickButtonPressed) {
//        if (sf::Joystick::isButtonPressed(0, 1)) {
            // FIXME handle gamestate change
//                return GameState::Main;
//        }
//    }

//		handleJoystick(Y);
    _FW.setGameState(handleMenuItems(event));
}

GameState HighscoreMenu::handleMenuItemResult(MenuResult result) {
    switch (result) {
        case MenuResult::Back:
            return GameState::Main;
            break;
    }
    return _MenuGameState;
}
