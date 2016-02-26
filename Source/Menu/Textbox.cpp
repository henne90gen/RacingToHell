#include "stdafx.h"
#include "Menu\Textbox.h"

Textbox::Textbox(sf::Vector2f Position, sf::Vector2f Size, int CharacterSize, std::string Text)
	: _FillColor(sf::Color(255, 255, 255)), _FillColorDisabled(sf::Color(140, 140, 140)), _OutlineColor(sf::Color(0, 0, 0)), _OutlineColorFocused(sf::Color(0, 150, 205)), _TextColor(sf::Color(0, 0, 0)),
	_isDisabled(false), _isFocused(false), _ShowCursor(true), _CursorPosition(0), _MaxLength(7)
{
	_Font.loadFromFile("Resources\\Font\\arial.ttf");

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

	setCursor();
}

Textbox::~Textbox()
{

}

void Textbox::update()
{
	if (_isDisabled)
	{
		_Box.setFillColor(_FillColorDisabled);
		_Box.setOutlineColor(_OutlineColor);
		_ShowCursor = false;
	}
	else
	{
		_Box.setFillColor(_FillColor);

		if (_isFocused)
		{
			_Box.setOutlineColor(_OutlineColorFocused);

			if (_CursorClock.getElapsedTime().asSeconds() > 0.6f)
			{
				_ShowCursor = !_ShowCursor;
				_CursorClock.restart();
			}
		}
		else
		{
			_Box.setOutlineColor(_OutlineColor);
			_ShowCursor = false;
		}
	}


}

void Textbox::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Box);
	RenderWindow.draw(_Text);

	if (_ShowCursor)
	{
		RenderWindow.draw(_Cursor);
	}

	if (!_isDisabled && MouseOverTextbox(sf::Mouse::getPosition(RenderWindow)))
	{
		sf::StandardCursor Cursor(sf::StandardCursor::TEXT);
		Cursor.set(RenderWindow.getSystemHandle());
	}
	else
	{
		sf::StandardCursor Cursor(sf::StandardCursor::NORMAL);
		Cursor.set(RenderWindow.getSystemHandle());
	}
}

void Textbox::handleEvent(sf::Event& Event)
{
	if (!_isDisabled && Event.type == sf::Event::MouseButtonPressed)
	{
		if (MouseOverTextbox(sf::Vector2i(Event.mouseButton.x, Event.mouseButton.y)))
		{
			_isFocused = true;
			_ShowCursor = true;
			_CursorClock.restart();

			_CursorPosition = 0;
			for (int i = _Text.getString().getSize(); i > 0; i--)
			{
				sf::Text TmpText = _Text;
				sf::Text TmpChar = _Text;

				TmpText.setString(_Text.getString().substring(0, i));
				TmpChar.setString(_Text.getString().substring(i - 1, 1));

				if (Event.mouseButton.x > TmpText.getPosition().x + TmpText.getLocalBounds().width - TmpChar.getLocalBounds().width / 2)
				{
					_CursorPosition = i;
					break;
				}
			}
			
			setCursor();
		}
		else
		{
			_isFocused = false;
		}
	}

	if (!_isDisabled && _isFocused && Event.type == sf::Event::KeyPressed)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		{
			if (_Text.getString().getSize() < _MaxLength)
			{
				if (Event.key.code < 26)
				{
					_Text.setString(_Text.getString().substring(0, _CursorPosition) + (char)(Event.key.code + 97 - 32 * (int)(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))) + _Text.getString().substring(_CursorPosition, _Text.getString().getSize() - _CursorPosition));
					_CursorPosition++;
					setCursor();
				}
				else if (Event.key.code < 36 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				{
					_Text.setString(_Text.getString().substring(0, _CursorPosition) + (char)(Event.key.code + 22) + _Text.getString().substring(_CursorPosition, _Text.getString().getSize() - _CursorPosition));
					_CursorPosition++;
					setCursor();
				}
			}

			if (Event.key.code == 59 && _Text.getString().getSize() > 0 && _CursorPosition > 0)
			{
				_Text.setString(_Text.getString().substring(0, _CursorPosition - 1) + _Text.getString().substring(_CursorPosition, _Text.getString().getSize() - _CursorPosition));
				_CursorPosition--;
				setCursor();
			}
		}

		if (_CursorPosition > 0 && Event.key.code == 71)
		{
			_CursorPosition--;
			setCursor();
		}

		if (_CursorPosition < _Text.getString().getSize() && Event.key.code == 72)
		{
			_CursorPosition++;
			setCursor();
		}
	}
}

void Textbox::setCursor()
{
	sf::Text TmpText = _Text;
	TmpText.setString(_Text.getString().substring(0, _CursorPosition));

	_Cursor.setPosition(_Text.getPosition() + sf::Vector2f(TmpText.getLocalBounds().width, 4));
}

bool Textbox::MouseOverTextbox(sf::Vector2i MousePosition)
{
	return MousePosition.x >= _Box.getPosition().x && MousePosition.x < _Box.getPosition().x + _Box.getSize().x && MousePosition.y > _Box.getPosition().y && MousePosition.y < _Box.getPosition().y + _Box.getSize().y;
}