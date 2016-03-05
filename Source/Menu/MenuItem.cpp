#include "stdafx.h"
#include "Menu\MenuItem.h"

MenuItem::MenuItem(MenuItems type, MenuResult action) : _Enabled(true), _Type(type), _Action(action)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
}

MenuItem::~MenuItem()
{
}

MenuResult MenuItem::getAction() {
	if (_Enabled) {
		return _Action;
	}
	return MenuResult::Nothing;
}