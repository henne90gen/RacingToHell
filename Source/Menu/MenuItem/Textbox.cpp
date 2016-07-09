#include "stdafx.h"
#include "Menu/MenuItem/Textbox.h"

Textbox::Textbox(sf::Vector2f Position, sf::Vector2f Size, int CharacterSize, std::string Text, bool isFocused)
	: MenuItem(MenuItemType::MTextbox, MenuResult::Nothing),
	_FillColor(sf::Color(255, 255, 255)), _FillColorDisabled(sf::Color(140, 140, 140)), _OutlineColor(sf::Color(0, 0, 0)), _OutlineColorFocused(sf::Color(0, 150, 205)), 
	_TextColor(sf::Color(0, 0, 0)), _ShowCursor(true), _CursorPosition(0)
{
	_CursorClock.restart();

	_Box.setFillColor(_FillColor);
	_Box.setOutlineColor(_OutlineColor);
	_Box.setOutlineThickness(2);
	_Box.setPosition(Position);
	_Box.setSize(Size);

	_Text.setFont(_Font);
	_Text.setCharacterSize(CharacterSize);
	_Text.setPosition(Position + sf::Vector2f(2, 0));
	_Text.setColor(_TextColor);
	_Text.setString(Text);

	_Cursor.setSize(sf::Vector2f(1, CharacterSize));
	_Cursor.setFillColor(sf::Color::Black);
	
	_Focused = true;

	setCursor();
}

void Textbox::render(sf::RenderWindow& RenderWindow)
{
	if (_Enabled) {
		_Box.setFillColor(_FillColor);

		if (_Focused) {
			_Box.setOutlineColor(_OutlineColorFocused);
			if (_CursorClock.getElapsedTime().asSeconds() > 0.6f) {
				_ShowCursor = !_ShowCursor;
				_CursorClock.restart();
			}
			_ShowCursor = true;
		}
		else {
			_Box.setOutlineColor(_OutlineColor);
			_ShowCursor = false;
		}
	}
	else {
		_Box.setFillColor(_FillColorDisabled);
		_Box.setOutlineColor(_OutlineColor);
		_ShowCursor = false;
	}

	RenderWindow.draw(_Box);
	RenderWindow.draw(_Text);

	if (_ShowCursor)
	{
		RenderWindow.draw(_Cursor);
	}
}

MenuResult Textbox::handleEvent(sf::Event & Event, sf::Vector2f MousePos)
{
	if (Event.type == sf::Event::MouseButtonPressed) {
		if (pointInRectangle(getRect(), MousePos)) {
			_Focused = true;
		}
		else {
			_Focused = false;
		}
		if (_Enabled && _Focused) {
			_ShowCursor = true;
			_CursorClock.restart();
			_CursorPosition = 0;

			for (int i = _Text.getString().getSize(); i > 0; i--) {
				sf::Text TmpText = _Text;
				sf::Text TmpChar = _Text;

				TmpText.setString(_Text.getString().substring(0, i));
				TmpChar.setString(_Text.getString().substring(i - 1, 1));

				if (Event.mouseButton.x > TmpText.getPosition().x + TmpText.getLocalBounds().width - TmpChar.getLocalBounds().width / 2) {
					_CursorPosition = i;
					break;
				}
			}
			setCursor();
		}
	}
	else if (Event.type == sf::Event::MouseMoved) {
		if (pointInRectangle(getRect(), MousePos)) {
			_Hovering = true;
		}
		else {
			_Hovering = false;
		}
	}
	else if (Event.type == sf::Event::KeyPressed) {
		if (_Enabled && _Focused) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				return MenuResult::SubmitScore;
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
				// Delete character with backspace
				if (Event.key.code == 59 && _Text.getString().getSize() > 0 && _CursorPosition > 0) {				_Text.setString(_Text.getString().substring(0, _CursorPosition - 1) + _Text.getString().substring(_CursorPosition, _Text.getString().getSize() - _CursorPosition));
					_CursorPosition--;
					setCursor();
				}
				// Delete character with delete
				else if (Event.key.code == 66 && _Text.getString().getSize() > 0 && _CursorPosition < _Text.getString().getSize()) {
					_Text.setString(_Text.getString().substring(0, _CursorPosition) + _Text.getString().substring(_CursorPosition + 1, _Text.getString().getSize() - _CursorPosition));
				}
			}

			// Moving cursor left with the left arrow
			if (_CursorPosition > 0 && Event.key.code == 71) {
				_CursorPosition--;
				setCursor();
			}

			// Moving cursor right with the right arrow
			if (_CursorPosition < _Text.getString().getSize() && Event.key.code == 72) {
				_CursorPosition++;
				setCursor();
			}
		}
	}
	else if (Event.type == sf::Event::TextEntered) {
		if (_Enabled && _Focused) {
			// Checking if typed character is valid: [0-9][A-Z][a-z]
			sf::Uint32 c = Event.text.unicode;
			if ((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)) {
				std::string newString = _Text.getString().substring(0, _CursorPosition) + static_cast<char>(c) + _Text.getString().substring(_CursorPosition, _Text.getString().getSize() - _CursorPosition);

				if (!isStringTooLarge(newString)) {
					_Text.setString(newString);
					_CursorPosition++;
					setCursor();
				}
			}
		}
	}
	return _Action;
}

void Textbox::setCursor()
{
	sf::Text TmpText = _Text;
	TmpText.setString(_Text.getString().substring(0, _CursorPosition));

	_Cursor.setPosition(_Text.getPosition() + sf::Vector2f(TmpText.getLocalBounds().width + 1, 4));
}

sf::FloatRect Textbox::getRect()
{
	return _Box.getGlobalBounds();
}

bool Textbox::isStringTooLarge(std::string str)
{
	sf::Text TmpText = _Text;
	TmpText.setString(str);

	return TmpText.getLocalBounds().width > _Box.getSize().x - 4;
}
