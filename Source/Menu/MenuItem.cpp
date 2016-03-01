#include "stdafx.h"
#include "Menu\MenuItem.h"


MenuItem::MenuItem(sf::Vector2f pos, sf::Vector2f size, MenuResult action) : _Action(action), _Enabled(true)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(40);

	bool centerText = false;

	switch (_Action) {
	case MenuResult::Resume:
		_Text.setString("Play");
		_Alignment = TextAlignment::Center;
		break;
	case MenuResult::PreviousSkin:
		_Text.setString("<<");
		_Alignment = TextAlignment::Left;
		break;
	case MenuResult::NextSkin:
		_Text.setString(">>");
		_Alignment = TextAlignment::Left;
		break;
	case MenuResult::Option:
		_Text.setString("Options");
		_Alignment = TextAlignment::Center;
		break;
	case MenuResult::Back:
		_Text.setString("Back");
		_Alignment = TextAlignment::Center;
		break;
	case MenuResult::Exit:
		_Text.setString("Exit");
		_Alignment = TextAlignment::Center;
		break;
	case MenuResult::BackToMain:
		_Text.setString("Back");
		_Alignment = TextAlignment::Left;
		break;
	case MenuResult::SubmitScore:
		_Text.setString("Submit");
		_Alignment = TextAlignment::Right;
		break;
	}

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
	default:
		break;
	}

	_Text.setPosition(_Background.getPosition() + sf::Vector2f(_Background.getLocalBounds().width / 2 - _Text.getLocalBounds().width / 2, 0));
}

MenuItem::~MenuItem()
{
}

void MenuItem::render(sf::RenderWindow & Window)
{
	Window.draw(_Background);
	Window.draw(_Text);

	sf::FloatRect rect = getRect();
	sf::Vector2i MousePos = sf::Mouse::getPosition(Window);

	switchHoverState(MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width, _JoystickSelected);
}

sf::FloatRect MenuItem::getRect()
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

MenuResult MenuItem::getAction()
{
	if (_Enabled) {
		return _Action;
	}
	return MenuResult::Nothing;
}

void MenuItem::switchHoverState(bool hoverState, bool joystickSelected)
{
	_Hovering = hoverState;
	_JoystickSelected = joystickSelected;

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
}
