#pragma once
#include "Menu/MenuItem/MenuItem.h"
class MenuButton : public MenuItem
{
public:
	/*
		Button element that can be placed in a menu
		@param pos Position of the Button on the screen
		@param size Size of the Button
		@param action Action the Button is associated with
		@param text Text that is going to be on the Button
		@param align Alignment of the text on the Button
	*/
	MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, std::string text, TextAlignment align);
	~MenuButton() {}

	/*
		See MenuItem for details
	*/
	void render(sf::RenderWindow& window);

	/*
		See MenuItem for details
	*/
	MenuResult handleEvent(sf::Event & newEvent, sf::Vector2f mousePos);

	/*
		See MenuItem for details
	*/
	sf::FloatRect getRect();

	/*
		@return std::string Text of the Button
	*/
	std::string getText() { return _Text.getString(); }

	/*
		Button doesn't have a value that could be changed
		@return bool False
	*/
	void setValue(float x) {}

	/*
		Button doesn't have a value that could be returned
		@return float 0.0f
	*/
	float getValue() { return 0.0f; }
	
	/*
		Button doesn't have a maximum value that could be returned
		@return float 0.0f
	*/
	float getMaxValue() { return 0.0f; }
private:
	sf::Text _Text;
	TextAlignment _Alignment;
	sf::RectangleShape _Background;
};

