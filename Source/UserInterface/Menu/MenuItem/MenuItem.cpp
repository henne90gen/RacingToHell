#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/MenuItem.h"

MenuItem::MenuItem(MenuItemType type, MenuResult action, sf::Font &font) : _Font(font), _Enabled(true), _Type(type),
                                                                           _Action(action), _Focused(false)
{
	sf::Uint32 MyCharset[] = { 0x25CF };
}

bool MenuItem::pointInRectangle(sf::FloatRect Rect, sf::Vector2f Position)
{
	return (Position.y > Rect.top && Position.y < Rect.top + Rect.height && Position.x > Rect.left && Position.x < Rect.left + Rect.width);
}
