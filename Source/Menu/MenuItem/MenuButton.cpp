#include "stdafx.h"
#include "Menu/MenuItem\MenuButton.h"

MenuButton::MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, std::string text, TextAlignment align) : MenuItem(MenuItemType::MButton, action), _Alignment(align)
{
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(0.8f * (float)size.y);
	_Text.setString(text);

	_Background.setPosition(pos);
	_Background.setSize(size);
	_Background.setOutlineThickness(1);
	_Background.setOutlineColor(sf::Color::Black);

	switch (_Alignment)
	{
	case TextAlignment::Center:
		_Background.setPosition(_Background.getPosition() - sf::Vector2f(_Background.getLocalBounds().width / 2, 0));
		break;
	case TextAlignment::Right:
		_Background.setPosition(_Background.getPosition() - sf::Vector2f(_Background.getLocalBounds().width, 0));
		break;
	}

	_Text.setPosition(_Background.getPosition() + sf::Vector2f(_Background.getLocalBounds().width / 2 - _Text.getLocalBounds().width / 2, 0));
}

void MenuButton::render(sf::RenderWindow & window)
{
	if ((_Hovering && _Enabled) || (_Focused && _Enabled)) {
		_Background.setFillColor(sf::Color(50, 50, 50, 100));
		_Text.setColor(sf::Color::White);
	}
	else if (_Enabled) {
		_Background.setFillColor(sf::Color(0, 0, 0, 175));
		_Text.setColor(sf::Color::White);
	}
	else {
		_Background.setFillColor(sf::Color(0, 0, 0, 100));
		_Text.setColor(sf::Color(150, 150, 150, 255));
	}

	if (_Background.getSize().x > 0)
	{ 
		window.draw(_Background);
	}
	
	window.draw(_Text);
}

MenuResult MenuButton::handleEvent(sf::Event & newEvent, sf::Vector2f mousePos)
{
	if (_Enabled) {
		if (newEvent.type == sf::Event::MouseButtonPressed) {
			if (mousePos.y > getRect().top && mousePos.y < getRect().top + getRect().height && mousePos.x > getRect().left && mousePos.x < getRect().left + getRect().width)
			{
				return _Action;
			}
		}
		else if (newEvent.type == sf::Event::JoystickButtonPressed) {
			if (newEvent.joystickButton.button == 0 && _Focused) {
				return _Action;
			}
		}
		else if (newEvent.type == sf::Event::MouseMoved) {
			if (mousePos.y > getRect().top && mousePos.y < getRect().top + getRect().height && mousePos.x > getRect().left && mousePos.x < getRect().left + getRect().width){
				_Hovering = true;
			}
			else {
				_Hovering = false;
			}

			_Focused = false; //?
		}
	}
	return MenuResult::Nothing;
}

sf::FloatRect MenuButton::getRect()
{
	if (_Action == MenuResult::NextSkin || _Action == MenuResult::PreviousSkin) {
		return _Text.getGlobalBounds();
	}
	else {
		return _Background.getGlobalBounds();
	}
}