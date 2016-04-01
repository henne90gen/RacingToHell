#include "stdafx.h"
#include "Menu/Menu.h"

Menu::Menu(GameState menuState) : _JoystickSelection(0), _JoystickDelay(0.15f), _MenuGameState(menuState)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	_Text.setFont(_Font);
}

void Menu::render(sf::RenderWindow & window)
{
	bool oneIsHovering = false;
	for (int i = 0; i < _MenuItems.size(); i++) {
		if (!oneIsHovering) {
			oneIsHovering = checkMenuItemHovered(window, i);
		}
		_MenuItems[i]->render(window);
	}
}

GameState Menu::handleMenuItems(sf::Event & Event)
{
	applyJoystickSelection(Event);

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

void Menu::applyJoystickSelection(sf::Event & Event)
{
	if (Event.type == sf::Event::MouseMoved && Event.type == sf::Event::JoystickMoved) {
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
}

bool Menu::checkMenuItemHovered(sf::RenderWindow& Window, int index)
{
	sf::StandardCursor Cursor;
	if (_MenuItems[index]->getHoverState() && _MenuItems[index]->getEnabled())
	{
		if (_MenuItems[index]->getType() == MenuItemType::MButton || _MenuItems[index]->getType() == MenuItemType::MSlider || _MenuItems[index]->getType() == MenuItemType::MCombobox || _MenuItems[index]->getType() == MenuItemType::MLobbyList) {
			Cursor.set(Window.getSystemHandle(), sf::StandardCursor::HAND);
			return true;
		}
		else if (_MenuItems[index]->getType() == MenuItemType::MTextbox) {
			Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TEXT);
			return true;
		}
		else {
			Cursor.set(Window.getSystemHandle(), sf::StandardCursor::NORMAL);
			return false;
		}
	}
	else {
		Cursor.set(Window.getSystemHandle(), sf::StandardCursor::NORMAL);
		return false;
	}
}