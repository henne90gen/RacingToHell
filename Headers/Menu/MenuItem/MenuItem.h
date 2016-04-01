#pragma once
#include "Menu/MenuItem/MenuResults.h"
#include "Menu/MenuItem/MenuItemType.h"
#include "Menu/AlignText.h"

class MenuItem
{
public:
	/*
		Interface for all the things that can be part of a menu
		@param type Type of MenuItem
		@param action Action the Button is associated with
	*/
	MenuItem(MenuItemType type, MenuResult action);
	~MenuItem() {}

	/*
		Renders the MenuItem to the specified RenderWindow
		@param window Window to draw to
	*/
	virtual void render(sf::RenderWindow & window) = 0;

	/*
		Handles events for the MenuItem
		@param newEvent Event to be handled
	*/
	virtual MenuResult handleEvent(sf::Event & newEvent, sf::Vector2f mousePos) = 0;

	/*
		@return sf::FloatRect Bounding box of the MenuItem
	*/
	virtual sf::FloatRect getRect() = 0;

	/*
		@return std::string Text of the MenuItem
	*/
	virtual std::string getText() = 0;

	virtual void setText(std::string str) {}

	/*
		@param vlaue New value for the MenuItem
		@return bool True if value was set successfully
	*/
	virtual void setValue(float value) = 0;

	/*
		@return float Value of the MenuItem
	*/
	virtual float getValue() = 0;

	/*
		@return float Maximum value for the MenuItem
	*/
	virtual float getMaxValue() = 0;

	/*
		@return bool True if the mouse is hovering over the MenuItem
	*/
	bool getHoverState() { return _Hovering; }

	/*
		@param hover New hover state
	*/
	void setHoverState(bool hover) { _Hovering = hover; }
	
	/*
		@return bool True if the MenuItem is enabled
	*/
	bool getEnabled() { return _Enabled; }

	/*
		@param enabled True for enabled
	*/
	void setEnabled(bool enabled) { _Enabled = enabled; }

	/*
		@return bool True if MenuItem is focused
	*/
	bool getFocused() { return _Focused; }

	/*
		@param focused New focus state
	*/
	void setFocused(bool focused) { _Focused = focused; }

	/*
		@param visible or not
	*/
	void setVisible(bool visible) { _Visible = visible; }

	/*
		@return MenuItemType
	*/

	virtual void resetTable() {}
	virtual void addPlayer(std::string name, bool admin) {}
	virtual void removePlayer(int index) {}
	virtual void setAdmin(bool admin) {}
	virtual void setMember(unsigned int index, bool ready, int score = -1, std::string name = "") {}
	MenuItemType getType() { return _Type; }
protected:
	sf::Font _Font;
	MenuItemType _Type;
	MenuResult _Action;
	bool _Hovering, _Enabled, _Focused, _Visible;

	bool pointInRectangle(sf::FloatRect Rect, sf::Vector2f Position);
};

