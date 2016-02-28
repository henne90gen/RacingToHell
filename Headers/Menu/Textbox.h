#pragma once

#include "StandardCursor.h"

class Textbox
{
public:
	Textbox(sf::Vector2f Position, sf::Vector2f Size, int CharacterSize, std::string Text = "");
	~Textbox();

	void update();
	void render(sf::RenderWindow& RenderWindow);
	void handleEvent(sf::Event& Event);

	std::string getText() { return _Text.getString(); }
	bool MouseOverTextbox(sf::Vector2i MousePosition);
	bool getDisabled() { return _isDisabled; }
private:
	sf::RectangleShape _Box, _Cursor;
	sf::Color _FillColor, _FillColorDisabled, _OutlineColor, _OutlineColorFocused, _TextColor;
	sf::Font _Font;
	sf::Text _Text;
	sf::Clock _CursorClock;

//	sf::StandardCursor _WindowsCursor;

	bool _isFocused, _isDisabled, _ShowCursor;
	int _CursorPosition;

	void setCursor();

	bool StringTooLarge(std::string str);
};