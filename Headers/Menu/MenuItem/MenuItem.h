#pragma once
#include "Menu/MenuItem/MenuResults.h"
#include "Menu/MenuItem/MenuItems.h"
#include "Menu/AlignText.h"

class MenuItem
{
public:
	MenuItem(MenuItems type, MenuResult action);
	~MenuItem();

	virtual void render(sf::RenderWindow & Window) = 0;
	virtual MenuResult handleEvent(sf::Event & Event, sf::Vector2f MousePos) = 0;

	virtual sf::FloatRect & getRect() = 0;

	bool getHoverState() { return _Hovering; }
	void setHoverState(bool hover) { _Hovering = hover; }
	
	bool getEnabled() { return _Enabled; }
	void setEnabled(bool enabled) { _Enabled = enabled; }

	bool getFocused() { return _Focused; }
	void setFocused(bool focused) { _Focused = focused; }

	MenuItems getType() { return _Type; }

protected:
	sf::Font _Font;
	MenuItems _Type;
	MenuResult _Action;
	bool _Hovering, _Enabled, _Focused;
};

