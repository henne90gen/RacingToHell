#include "stdafx.h"
#include "Menu/Menu.h"

Menu::Menu(GameState menuState) : _JoystickSelection(0), _JoystickDelay(0.15f), _MenuGameState(menuState)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	_Text.setFont(_Font);
}

GameState Menu::handleMenuItems(sf::Event & Event)
{
	applyJoystickSelection();

	if (Event.type == sf::Event::MouseButtonPressed) {
		_MousePos = sf::Vector2f(Event.mouseButton.x, Event.mouseButton.y);
	}
	else if (Event.type == sf::Event::MouseMoved) {
		_MousePos = sf::Vector2f(Event.mouseMove.x, Event.mouseMove.y);
	}

	if (_Event.type == sf::Event::Closed) {
		return GameState::Exiting;
	}
	else {
		GameState result = _MenuGameState;
		for (int i = 0; i < _MenuItems.size(); i++) {
			if (result == _MenuGameState) {
				result = handleMenuItemResult(_MenuItems[i]->handleEvent(_Event, _MousePos));
			}
		}
		return result;
	}
	return _MenuGameState;
}

void Menu::handleJoystick(float axis)
{
	if (axis < 10 && axis > -10) {
		_JoystickTimer.restart();
	}

	if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
		if (axis < -50 && _JoystickSelection > 0) {
			_JoystickSelection--;
			_JoystickTimer.restart();
		}
		else if (axis > 50 && _JoystickSelection < _MenuItems.size() - 3) {
			_JoystickSelection++;
			_JoystickTimer.restart();
		}
	}
}

void Menu::applyJoystickSelection()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		if (_MenuItems[i]->getType() != MenuItemType::MTextbox) {
			if (i == _JoystickSelection) {
				_MenuItems[i]->setFocused(true);
			}
			else {
				_MenuItems[i]->setFocused(false);
			}
		}
	}
}

void Menu::checkMenuItemHovered(sf::RenderWindow& Window)
{
	sf::StandardCursor Cursor;
	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		if (_MenuItems[i]->getHoverState() && _MenuItems[i]->getEnabled())
		{
			switch (_MenuItems[i]->getType()) {
			case MenuItemType::MButton:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::HAND);
				break;
			case MenuItemType::MTextbox:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TEXT);
				break;
			case MenuItemType::MSlider:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::HAND);
				break;
			}
			return;
		}
	}
	Cursor.set(Window.getSystemHandle(), sf::StandardCursor::NORMAL);
}