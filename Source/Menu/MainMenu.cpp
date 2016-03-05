#include "stdafx.h"
#include "Menu\MainMenu.h"

MainMenu::MainMenu()
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume, "Play", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit, "Exit", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::PreviousSkin, "<<", TextAlignment::Left));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 0), MenuResult::NextSkin, ">>", TextAlignment::Left));

	//Main-Menu Text
	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

	//Stats-Box
	_StatBox = new StatBox(sf::Vector2f(SCREENWIDTH / 2, 575));
}

MainMenu::~MainMenu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}

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
		if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
		else if (_Event.type == sf::Event::MouseButtonPressed || _Event.type == sf::Event::JoystickButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
			for (int i = 0; i < _MenuItems.size(); i++) {
				sf::FloatRect rect = _MenuItems[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width || 
					sf::Joystick::isButtonPressed(0, 0)) 
				{
					if (sf::Joystick::isButtonPressed(0, 0)) {
						i = _JoystickSelection;
					}
					switch (_MenuItems[i]->getAction()) {
					case MenuResult::Resume:
						return GameState::Running;
						break;
					case MenuResult::Option:
						return GameState::Options;
						break;
					case MenuResult::Nothing:
						return GameState::Main;
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
				}
			}
		}
		else if (_Event.type == sf::Event::JoystickMoved && _JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
			float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
			_MenuItems[_JoystickSelection]->switchHoverState(false, false);
			if (Y < -80) {
				if (_JoystickSelection > 0) {
					_JoystickSelection--;
				}
			}
			else if (Y > 80) {
				if (_JoystickSelection < _MenuItems.size() - 3) {
					_JoystickSelection++;
				}
			}
			else if (X < -80) {
				_SelectedCar--;
			}
			else if (X > 80) {
				_SelectedCar++;
			}
			_MenuItems[_JoystickSelection]->switchHoverState(true, true);
			_JoystickTimer.restart();
		}
		else if (_Event.type == sf::Event::MouseMoved) {
			_MenuItems[_JoystickSelection]->switchHoverState(false, false);
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < 10 && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > -10) {
			_JoystickTimer.restart();
		}
	}
	return GameState::Main;
}

GameState MainMenu::handleMenuItemAction(int index)
{
	return GameState();
}
