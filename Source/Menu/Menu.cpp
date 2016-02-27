#include "stdafx.h"
#include "Menu/Menu.h"

Menu::Menu()
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	_Text.setFont(_Font);
}

Menu::~Menu()
{
}

void Menu::checkMouseHover(sf::RenderWindow& Window)
{
	sf::Vector2i MousePos = sf::Mouse::getPosition(Window);
	for (int i = 0; i < _MenuItems.size(); i++) {
		sf::FloatRect rect = _MenuItems[i]->getRect();
		if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
			_MenuItems[i]->switchHoverState(true);
		}
		else {
			_MenuItems[i]->switchHoverState(false);
		}
	}

	if (MenuItemHovered())
	{
		sf::StandardCursor Cursor(sf::StandardCursor::HAND);
		Cursor.set(Window.getSystemHandle());
	}
	else
	{
		sf::StandardCursor Cursor(sf::StandardCursor::NORMAL);
		Cursor.set(Window.getSystemHandle());
	}
}

bool Menu::MenuItemHovered()
{
	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		if (_MenuItems[i]->getHoverState())
		{
			return true;
		}
	}

	return false;
}