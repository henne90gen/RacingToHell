#include "stdafx.h"
#include "UserInterface/Menu/PauseMultiplayerMenu.h"
#include "Framework/Framework.h"

PauseMultiplayerMenu::PauseMultiplayerMenu(Framework &framework) : Menu(framework,
                                                                                               GameState::PauseMultiplayer) {
    _Background.setFillColor(sf::Color(0, 0, 0, 100));
    _Background.setSize(sf::Vector2f(SCREENWIDTH, SCREENHEIGHT));
    _Background.setPosition(sf::Vector2f(0, 0));

    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);
//    std::shared_ptr<MenuButton> button1(
//            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume, "Resume",
//                           TextAlignment::Center));
//    _MenuItems.push_back(button1);
//    std::shared_ptr<MenuButton> button2(
//            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options",
//                           TextAlignment::Center));
//    _MenuItems.push_back(button2);
//    std::shared_ptr<MenuButton> button3(
//            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Back, "Leave this Game",
//                           TextAlignment::Center));
//    _MenuItems.push_back(button3);
//    std::shared_ptr<MenuButton> button4(
//            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 450), ButtonSize, MenuResult::Exit, "Exit",
//                           TextAlignment::Center));
//    _MenuItems.push_back(button4);

    //Menu-Text
    _Text.setString("Pause Menu");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}


PauseMultiplayerMenu::~PauseMultiplayerMenu() {
}

void PauseMultiplayerMenu::render(sf::RenderWindow &window) {
    window.draw(_Background);

    window.draw(_Text);
}

void PauseMultiplayerMenu::handleEvent(sf::Event &event) {

//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
//
//		if (_Event.type == sf::Event::JoystickButtonPressed) {
//			if (sf::Joystick::isButtonPressed(0, 1)) {
//				return GameState::Running;
//			}
//		}
//
//		handleJoystick(Y);

//    switch (getMenuItemResult(event)) {
//        case MenuResult::Resume:
//            _FW.goBackGameState();
//            break;
//        case MenuResult::Option:
//            _FW.advanceToGameState(GameState::Options);
//            break;
//        case MenuResult::Back:
//            _FW.advanceToGameState(GameState::MainMenu);
//            break;
//        case MenuResult::Exit:
//            _FW.advanceToGameState(GameState::Exiting);
//            break;
//        default:
//            break;
//    }
}
