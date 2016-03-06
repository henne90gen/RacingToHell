#include "stdafx.h"
#include "Menu/Menu.h"

Menu::Menu() : _JoystickSelection(0), _JoystickDelay(0.15f)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	_Text.setFont(_Font);
}

Menu::~Menu()
{
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