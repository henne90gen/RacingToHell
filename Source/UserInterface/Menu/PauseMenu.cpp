#include "stdafx.h"
#include "UserInterface/Menu/PauseMenu.h"
#include "Framework/Framework.h"

PauseMenu::PauseMenu(Framework &framework) : Menu(framework, GameState::Pause) {

    sf::Font &font = _FW.getOptionsManager().getFont();

    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

    std::shared_ptr<MenuButton> resumeBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize,
                                                                         MenuResult::Resume, font, "Resume",
                                                                         TextAlignment::Center);
    _MenuItems.push_back(resumeBtn);

    std::shared_ptr<MenuButton> optionsBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 310),
                                                                          ButtonSize, MenuResult::Option, font,
                                                                          "Options", TextAlignment::Center);
    _MenuItems.push_back(optionsBtn);

    std::shared_ptr<MenuButton> backToMainBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 380),
                                                                             ButtonSize,
                                                                             MenuResult::Back, font, "Main Menu",
                                                                             TextAlignment::Center);
    _MenuItems.push_back(backToMainBtn);

    std::shared_ptr<MenuButton> exitBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 450), ButtonSize,
                                                                       MenuResult::Exit, font, "Exit",
                                                                       TextAlignment::Center);
    _MenuItems.push_back(exitBtn);

    //Menu-Text
    _Text.setString("Pause Menu");
    _Text.setCharacterSize(53);
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

    switch (getMenuItemResult(event)) {
        case MenuResult::Resume:
            _FW.getLevelManager().startMoving();
            _FW.goBackGameState();
            break;
        case MenuResult::Option:
            _FW.advanceToGamState(GameState::Options);
            break;
        case MenuResult::Back:
            _FW.reset();
            _FW.advanceToGamState(GameState::MainMenu);
            break;
        case MenuResult::Exit:
            _FW.advanceToGamState(GameState::Exiting);
            break;
        default:
            break;
    }
}
