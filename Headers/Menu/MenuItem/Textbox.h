#pragma once
#include "StandardCursor.h"
#include "Menu/MenuItem/MenuItem.h"

class Textbox : public MenuItem
{
public:
	Textbox(sf::Vector2f Position, sf::Vector2f Size, int CharacterSize, std::string Text, bool isFocused);
	~Textbox();

	void render(sf::RenderWindow& RenderWindow);
	MenuResult handleEvent(sf::Event & Event, sf::Vector2f MousePos);

	std::string getText() { return _Text.getString(); }
	sf::FloatRect& getRect();
	
private:
	sf::RectangleShape _Box, _Cursor;
	sf::Color _FillColor, _FillColorDisabled, _OutlineColor, _OutlineColorFocused, _TextColor;
	sf::Text _Text;
	sf::Clock _CursorClock;

//	sf::StandardCursor _WindowsCursor;

	bool _ShowCursor;
	int _CursorPosition;

	void setCursor();

	bool StringTooLarge(std::string str);
};