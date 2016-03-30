#include "stdafx.h"
#include "Menu\MainMenu.h"

MainMenu::MainMenu() : Menu(GameState::Main)
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

	std::shared_ptr<MenuButton> button1(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 100), ButtonSize, MenuResult::Resume, "Play", TextAlignment::Center));
	_MenuItems.push_back(button1);
	std::shared_ptr<MenuButton> button2(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 170), ButtonSize, MenuResult::Multiplayer, "Multiplayer", TextAlignment::Center));
	_MenuItems.push_back(button2);
	std::shared_ptr<MenuButton> button3(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Highscore, "Highscores", TextAlignment::Center));
	_MenuItems.push_back(button3);
	std::shared_ptr<MenuButton> button4(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options", TextAlignment::Center));
	_MenuItems.push_back(button4);
	std::shared_ptr<MenuButton> button5(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit, "Exit", TextAlignment::Center));
	_MenuItems.push_back(button5);
	std::shared_ptr<MenuButton> button6(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::PreviousSkin, "<<", TextAlignment::Left));
	_MenuItems.push_back(button6);
	std::shared_ptr<MenuButton> button7(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::NextSkin, ">>", TextAlignment::Left));
	_MenuItems.push_back(button7);

	//Main-Menu Text
	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 20));

	//Stats-Box
	_StatBox = std::make_shared<StatBox>(sf::Vector2f(SCREENWIDTH / 2, 575));
}

void MainMenu::render(sf::RenderWindow& window)
{
	window.draw(_CarSkin);
	_StatBox->render(window, _SelectedCar);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(window);
	}

	window.draw(_Text);

	checkMenuItemHovered(window);
}

GameState MainMenu::handleEvents(sf::RenderWindow& window)
{
	while (window.pollEvent(_Event)) {

		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		handleJoystick(Y);

		if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay - 0.05f) {
			if (X < -50) {
				_SelectedCar--;
				_JoystickTimer.restart();
			}
			else if (X > 50) {
				_SelectedCar++;
				_JoystickTimer.restart();
			}
		}

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState MainMenu::handleMenuItemResult(MenuResult result)
{
	switch (result) {
	case MenuResult::Resume:
		return GameState::Running;
		break;
	case MenuResult::Highscore:
		return GameState::Highscores;
		break;
	case MenuResult::Option:
		return GameState::Options;
		break;
	case MenuResult::PreviousSkin:
		_SelectedCar--;
		break;
	case MenuResult::NextSkin:
		_SelectedCar++;
		break;
	case MenuResult::Exit:
		return GameState::Exiting;
		break;
	case MenuResult::Multiplayer:
		return GameState::MultiplayerSelection;
		break;
	}
	return _MenuGameState;
}
