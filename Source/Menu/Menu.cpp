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

bool Menu::MenuItemHovered()
{
	for (unsigned int i = 0; i < _MenuItems.size(); i++)
	{
		if (_MenuItems[i]->getHoverState() && _MenuItems[i]->getEnabled())
		{
			return true;
		}
	}
	return false; 
}