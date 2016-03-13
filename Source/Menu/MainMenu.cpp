#include "stdafx.h"
#include "Menu\MainMenu.h"

MainMenu::MainMenu() : Menu(GameState::Main)
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 170), ButtonSize, MenuResult::Resume, "Play", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Highscore, "Highscores", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit, "Exit", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::PreviousSkin, "<<", TextAlignment::Left));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::NextSkin, ">>", TextAlignment::Left));

	//Main-Menu Text
	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 70));

	//Stats-Box
	_StatBox = new StatBox(sf::Vector2f(SCREENWIDTH / 2, 575));
}

MainMenu::~MainMenu()
{
	delete _StatBox;
	_StatBox = nullptr;
}

void MainMenu::render(sf::RenderWindow& Window)
{
	Window.draw(_CarSkin);
	_StatBox->render(Window, _SelectedCar);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	Window.draw(_Text);

	checkMenuItemHovered(Window);
}

GameState MainMenu::handleEvents(sf::RenderWindow& Window)
{
	while (Window.pollEvent(_Event)) {

		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if (Y < 10 && Y > -10) {
			_JoystickTimer.restart();
		}

		if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
			if (Y < -50 && _JoystickSelection > 0) {
				_JoystickSelection--;
				_JoystickTimer.restart();
			}
			else if (Y > 50 && _JoystickSelection < _MenuItems.size() - 3) {
				_JoystickSelection++;
				_JoystickTimer.restart();
			}
		}

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
	}
	return _MenuGameState;
}
