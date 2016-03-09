#include "stdafx.h"
#include "Menu/PauseMenu.h"


PauseMenu::PauseMenu() : Menu(GameState::Pause)
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
		return handleMenuItems(_Event);
			/*if (_Event.type == sf::Event::MouseButtonPressed) {
			for (int i = 0; i < _MenuItems.size(); i++) {
				sf::FloatRect rect = _MenuItems[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) 
				{
					return handleMenuItemAction(i);
				}
			}
		}
		else if (_Event.type == sf::Event::JoystickButtonPressed) {
			if (sf::Joystick::isButtonPressed(0, 0)) {
				return handleMenuItemAction(_JoystickSelection);
			}
			else if (sf::Joystick::isButtonPressed(0, 1)) {
				return GameState::Running;
			}
		}
		else if (_Event.type == sf::Event::MouseMoved) {
			for (unsigned int i = 0; i < _MenuItems.size(); i++) {
				sf::FloatRect rect = _MenuItems[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width)
				{
					_MenuItems[i]->switchHoverState(true, false, true);
				}
				else {
					_MenuItems[i]->switchHoverState(false, false, false);
				}
			}
		}
		
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < 10 && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > -10) {
			_JoystickTimer.restart();
		}
	}

	if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		if (Y < -80 && _JoystickSelection > 0) {
			_MenuItems[_JoystickSelection]->switchHoverState(false, false, false);
			_JoystickSelection--;
			_MenuItems[_JoystickSelection]->switchHoverState(true, true, true);
			_JoystickTimer.restart();
		}
		else if (Y > 80 && _JoystickSelection < _MenuItems.size() - 1) {
			_MenuItems[_JoystickSelection]->switchHoverState(false, false, false);
			_JoystickSelection++;
			_MenuItems[_JoystickSelection]->switchHoverState(true, true, true);
			_JoystickTimer.restart();
		}*/
	}

	return GameState::Pause;
}

GameState PauseMenu::handleMenuItemResult(MenuResult result)
{
	switch (result) {
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
	return GameState::Pause;
}
