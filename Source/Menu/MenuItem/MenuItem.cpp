#include "stdafx.h"
#include "Menu/MenuItem/MenuItem.h"

MenuItem::MenuItem(MenuItemType type, MenuResult action) : _Enabled(true), _Type(type), _Action(action), _Focused(false)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
}

bool MenuItem::pointInRectangle(sf::FloatRect Rect, sf::Vector2f Position)
{
	return (Position.y > Rect.top && Position.y < Rect.top + Rect.height && Position.x > Rect.left && Position.x < Rect.left + Rect.width);
}
