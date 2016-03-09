#include "stdafx.h"
#include "Menu/Menu.h"

Menu::Menu(GameState menuState) : _JoystickSelection(0), _JoystickDelay(0.15f), _MenuGameState(menuState)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	_Text.setFont(_Font);
}

Menu::~Menu()
{
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

void Menu::applyJoystickSelection()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		if (i == _JoystickSelection) {
			_MenuItems[i]->setFocused(true);
		}
		else {
			_MenuItems[i]->setFocused(false);
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
			case MenuItems::MButton:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::HAND);
				break;
			case MenuItems::MTextbox:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TEXT);
				break;
			case MenuItems::MSlider:
				Cursor.set(Window.getSystemHandle(), sf::StandardCursor::HAND);
				break;
			}
			return;
		}
	}
	Cursor.set(Window.getSystemHandle(), sf::StandardCursor::NORMAL);
}