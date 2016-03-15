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
		Renders the Button to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Handles events for the Button
		@param newEvent Event to be handled
	*/
	MenuResult handleEvent(sf::Event & newEvent, sf::Vector2f mousePos);

	/*
		@return sf::FloatRect Bounding box of the Button
	*/
	sf::FloatRect & getRect();
private:
	sf::Text _Text;
	TextAlignment _Alignment;
	sf::RectangleShape _Background;
};

