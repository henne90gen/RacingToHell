#include "stdafx.h"
#include "Screen/Menu/PauseMenu.h"
#include "Framework/Framework.h"

PauseMenu::PauseMenu(Framework &framework) : Menu(framework, GameState::Pause) {


    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);
    std::shared_ptr<MenuButton> button1(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume, "Resume",
                           TextAlignment::Center));
    _MenuItems.push_back(button1);
    std::shared_ptr<MenuButton> button2(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options",
                           TextAlignment::Center));
    _MenuItems.push_back(button2);
    std::shared_ptr<MenuButton> button3(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Back, "Main Menu",
                           TextAlignment::Center));
    _MenuItems.push_back(button3);
    std::shared_ptr<MenuButton> button4(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 450), ButtonSize, MenuResult::Exit, "Exit",
                           TextAlignment::Center));
    _MenuItems.push_back(button4);

    //Menu-Text
    _Text.setString("Pause Menu");
    _Text.setCharacterSize(53);
    _Text.setColor(sf::Color::White);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}

void PauseMenu::render(sf::RenderWindow &window) {
    window.draw(_Text);

    Menu::render(window);
}

void PauseMenu::handleEvent(sf::Event &event) {
//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

//		if (_Event.type == sf::Event::JoystickButtonPressed) {
//			if (sf::Joystick::isButtonPressed(0, 1)) {
//				return GameState::Running;
//			}
//		}

//		handleJoystick(Y);

    // FIXME handle gamestate changes
    _FW.setGameState(handleMenuItems(event));
}

GameState PauseMenu::handleMenuItemResult(MenuResult result) {
    switch (result) {
        case MenuResult::Resume:
//            _FW.goBackToLastGameState();
        case MenuResult::Option:
            return GameState::Options;
        case MenuResult::Back:
            _FW.reset();
            return GameState::MainMenu;
        case MenuResult::Exit:
            return GameState::Exiting;
    }
    return _MenuGameState;
}
