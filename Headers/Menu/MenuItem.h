#pragma once
#include "Menu/MenuResults.h"
#include "Menu/MenuItems.h"
#include "Menu/AlignText.h"

class MenuItem
{
public:
	MenuItem(MenuItems type, MenuResult action);
	~MenuItem();

	virtual void render(sf::RenderWindow& Window) = 0;
	
	virtual sf::FloatRect getRect() = 0;
	MenuResult getAction();

	bool getHoverState() { return _Hovering; }
	virtual void switchHoverState(bool hoverState, bool joystickSelected) = 0;
	
	bool getEnabled() { return _Enabled; }
	void setEnabled(bool enabled) { _Enabled = enabled; }

	MenuItems getType() { return _Type; }

protected:
	sf::Font _Font;
	MenuItems _Type;
	MenuResult _Action;
	bool _Hovering, _Enabled, _JoystickSelected;
};

