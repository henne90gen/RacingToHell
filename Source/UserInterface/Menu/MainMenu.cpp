#include "stdafx.h"
#include "UserInterface/Menu/MainMenu.h"
#include "Framework/Framework.h"

MainMenu::MainMenu(Framework &framework) : Menu(framework, GameState::MainMenu) {
    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

    std::shared_ptr<MenuButton> button1(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 100), ButtonSize, MenuResult::Resume, "Play",
                           TextAlignment::Center));
    _MenuItems.push_back(button1);

    std::shared_ptr<MenuButton> button2(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 170), ButtonSize, MenuResult::Multiplayer, "Multiplayer",
                           TextAlignment::Center));
    button2->setEnabled(false);
    _MenuItems.push_back(button2);

    std::shared_ptr<MenuButton> button3(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Highscore, "Highscores",
                           TextAlignment::Center));
    _MenuItems.push_back(button3);

    std::shared_ptr<MenuButton> button4(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options",
                           TextAlignment::Center));
    _MenuItems.push_back(button4);

    std::shared_ptr<MenuButton> button5(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit, "Exit",
                           TextAlignment::Center));
    _MenuItems.push_back(button5);

    std::shared_ptr<MenuButton> button6(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 50),
                           MenuResult::PreviousSkin, "<<", TextAlignment::Left));
    _MenuItems.push_back(button6);

    std::shared_ptr<MenuButton> button7(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 50),
                           MenuResult::NextSkin, ">>", TextAlignment::Left));
    _MenuItems.push_back(button7);

    //MainMenu-Menu Text
    _Text.setString("Main Menu");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 20));

    //Stats-Box
    _StatBox = std::make_shared<StatBox>(sf::Vector2f(SCREENWIDTH / 2, 575));
}

void MainMenu::render(sf::RenderWindow &window) {
    _StatBox->setCarStats(_FW.getGOM().getPlayerCar()->getPlayerCarIndex());
    _StatBox->render(window);
    window.draw(_Text);

    Menu::render(window);
}

void MainMenu::handleEvent(sf::Event &event) {

//		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

//		handleJoystick(Y);

//		if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay - 0.05f) {
//			if (X < -50) {
//				_PlayerCarIndex--;
//				_JoystickTimer.restart();
//			}
//			else if (X > 50) {
//				_PlayerCarIndex++;
//				_JoystickTimer.restart();
//			}
//		}
    switch (getMenuItemResult(event)) {
        case MenuResult::Resume:
            _FW.advanceToGamState(GameState::Running);
            break;
        case MenuResult::Highscore:
            _FW.advanceToGamState(GameState::Highscores);
            break;
        case MenuResult::Option:
            _FW.advanceToGamState(GameState::Options);
            break;
        case MenuResult::PreviousSkin:
            _FW.getGOM().previousPlayerCar();
            break;
        case MenuResult::NextSkin:
            _FW.getGOM().nextPlayerCar();
            break;
        case MenuResult::Exit:
            _FW.advanceToGamState(GameState::Exiting);
            break;
        case MenuResult::Multiplayer:
            _FW.advanceToGamState(GameState::MultiplayerSelection);
            break;
        default:
            break;
    }
}
