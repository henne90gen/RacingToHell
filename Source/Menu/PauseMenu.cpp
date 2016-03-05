#include "stdafx.h"
#include "Menu/PauseMenu.h"


PauseMenu::PauseMenu()
{
	//Menu-Items
	sf::Vector2f ButtonSize = sf::Vector2f(150, 50);

	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 240), ButtonSize, MenuResult::Resume, "Resume", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 310), ButtonSize, MenuResult::Option, "Options", TextAlignment::Center));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize, MenuResult::Exit, "Exit", TextAlignment::Center));

	//Menu-Text
	_Text.setString("Pause Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::render(sf::RenderWindow & Window)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_Text);

	checkMenuItemHovered(Window);
}

GameState PauseMenu::handleEvents(sf::RenderWindow & Window)
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
						return GameState::Pause;
						break;
					case MenuResult::Exit:
						return GameState::Exiting;
						break;
					}
				}
			}
		}
		else if (_Event.type == sf::Event::JoystickMoved && _JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
			_MenuItems[_JoystickSelection]->switchHoverState(false, false);
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -80) {
				if (_JoystickSelection > 0) {
					_JoystickSelection--;
				}
			}
			else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 80) {
				if (_JoystickSelection < _MenuItems.size() - 1) {
					_JoystickSelection++;
				}
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
	return GameState::Pause;
}

GameState PauseMenu::handleMenuItemAction(int index)
{
	return GameState();
}
