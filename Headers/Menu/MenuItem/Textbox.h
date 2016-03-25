#pragma once
#include "StandardCursor.h"
#include "Menu/MenuItem/MenuItem.h"

class Textbox : public MenuItem
{
public:
	/*
		Textbox element that can be placed in a menu
		@param pos Position of the Textbox
		@param size Size of the Textbox
		@param charSize Size of the chars in the Textbox
		@param text Text that will be displayed in the Textbox by default
		@param isFocused Whether the Textbox is focused right after initialization
	*/
	Textbox(sf::Vector2f pos, sf::Vector2f size, int charSize, std::string text, bool isFocused);
	~Textbox() {}

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
	sf::FloatRect& getRect();

	/*
		@return std::string Text of the Textbox
	*/
	std::string getText() { return _Text.getString(); }

	/*
		Textbox doesn't have a value that could be changed
		@return bool False
	*/
	void setValue(float x) {}

	/*
		Textbox doesn't have a value that could be returned
		@return float 0.0f
	*/
	float getValue() { return 0.0f; }

	/*
		Button doesn't have a maximum value that could be returned
		@return float 0.0f
	*/
	float getMaxValue() { return 0.0f; }
private:
	sf::RectangleShape _Box, _Cursor;
	sf::Color _FillColor, _FillColorDisabled, _OutlineColor, _OutlineColorFocused, _TextColor;
	sf::Text _Text;
	sf::Clock _CursorClock;

	bool _ShowCursor;
	int _CursorPosition;

	/*
		Sets the cursor to the correct position in the text of the Textbox
	*/
	void setCursor();

	/*
		@param str String that is going to be checked
		@return bool True if str is too long for the Textbox
	*/
	bool isStringTooLarge(std::string str);
};