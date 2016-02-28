#include "stdafx.h"
#include "Menu\MenuItem.h"


MenuItem::MenuItem(sf::Vector2f pos, MenuResult action) : _Action(action)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(45);

	bool centerText = false;

	switch (_Action) {
	case MenuResult::Resume:
		_Text.setString("Play");
		centerText = true;
		break;
	case MenuResult::PreviousSkin:
		_Text.setString("<<");
		break;
	case MenuResult::NextSkin:
		_Text.setString(">>");
		break;
	case MenuResult::Option:
		_Text.setString("Options");
		centerText = true;
		break;
	case MenuResult::Back:
		_Text.setString("Back");
		centerText = true;
		break;
	case MenuResult::Exit:
		_Text.setString("Exit");
		centerText = true;
		break;
	case MenuResult::BackToMain:
		_Text.setString("Back");
		centerText = false;
		break;
	case MenuResult::SubmitScore:
		_Text.setString("Submit");
		centerText = false;
		break;
	}

	if (centerText) {
		_Text.setPosition(_Text.getPosition() - sf::Vector2f(_Text.getLocalBounds().width / 2, 0));
	}

	//Initializing rectangle that will be shown at hover-over
	//Position is adjusted by 10 down and size is increased by 10 horizontally and 7 vertically
	if (_Action == MenuResult::NextSkin || _Action == MenuResult::PreviousSkin) {
		_HoverRect.setPosition(_Text.getPosition() + sf::Vector2f(0, 15));
		_HoverRect.setSize(sf::Vector2f(_Text.getLocalBounds().width + 10, _Text.getLocalBounds().height + 7));
		_HoverRect.setFillColor(sf::Color::Transparent);
		_HoverRect.setOutlineThickness(5);
		_HoverRect.setOutlineColor(sf::Color::Black);
	}
	else {
		_HoverRect.setPosition(_Text.getPosition() + sf::Vector2f(0, 10));
		_HoverRect.setSize(sf::Vector2f(_Text.getLocalBounds().width + 10, _Text.getLocalBounds().height + 7));
		_HoverRect.setFillColor(sf::Color::Transparent);
		_HoverRect.setOutlineThickness(5);
		_HoverRect.setOutlineColor(sf::Color::Black);
	}
}

MenuItem::~MenuItem()
{
}

void MenuItem::render(sf::RenderWindow & Window)
{
	if (_Hovering) {
		Window.draw(_HoverRect);
	}
	Window.draw(_Text);
}

sf::FloatRect MenuItem::getRect()
{
	return _Text.getGlobalBounds();
}

void MenuItem::switchHoverState(bool hoverState)
{
	_Hovering = hoverState;
}
