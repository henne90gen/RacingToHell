#include "stdafx.h"
#include "Menu\MenuItem\MenuItem.h"

MenuItem::MenuItem(MenuItems type, MenuResult action) : _Enabled(true), _Type(type), _Action(action), _Focused(false)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
}