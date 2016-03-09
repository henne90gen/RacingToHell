#include "stdafx.h"
#include "Menu/MenuButton.h"

MenuButton::MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, std::string text, TextAlignment align) : MenuItem(MenuItems::MButton, action), _Alignment(align)
{
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(40);
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


MenuButton::~MenuButton()
{
}

void MenuButton::render(sf::RenderWindow & Window)
{
	if ((_Hovering && _Enabled) || (_JoystickSelected && _Enabled)) {
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

	Window.draw(_Background);
	Window.draw(_Text);

	sf::FloatRect rect = getRect();
	sf::Vector2i MousePos = sf::Mouse::getPosition(Window);

	switchHoverState(MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width, _JoystickSelected);
}

sf::FloatRect MenuButton::getRect()
{
	if (_Action == MenuResult::NextSkin || _Action == MenuResult::PreviousSkin)
	{
		return _Text.getGlobalBounds();
	}
	else
	{
		return _Background.getGlobalBounds();
	}
}

void MenuButton::switchHoverState(bool hoverState, bool joystickSelected)
{
	_Hovering = hoverState;
	_JoystickSelected = joystickSelected;
}