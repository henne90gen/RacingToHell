#include "stdafx.h"
#include "Menu\MenuItem.h"


MenuItem::MenuItem(sf::Vector2f pos, MenuResult action) : _Action(action)
{
	_Font.loadFromFile("Resources/Font/arial.ttf");
	
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(45);

	switch (_Action) {
	case MenuResult::Resume:
		_Text.setString("Play");
		_Text.setPosition(_Text.getPosition() - sf::Vector2f(_Text.getLocalBounds().width / 2, 0));
		break;
	case MenuResult::PreviousSkin:
		_Text.setString("<<");
		break;
	case MenuResult::NextSkin:
		_Text.setString(">>");
		break;
	case MenuResult::Exit:
		_Text.setString("Exit");
		_Text.setPosition(_Text.getPosition() - sf::Vector2f(_Text.getLocalBounds().width / 2, 0));
		break;
	}

	//Initializing rectangle that will be shown at hover-over
	//Position is adjusted by 10 down and size is increased by 10 horizontally and 7 vertically
	if (_Action == MenuResult::Resume || _Action == MenuResult::Exit) {
		_HoverRect.setPosition(_Text.getPosition() + sf::Vector2f(0, 10));
		_HoverRect.setSize(sf::Vector2f(_Text.getLocalBounds().width + 10, _Text.getLocalBounds().height + 7));
		_HoverRect.setFillColor(sf::Color::Transparent);
		_HoverRect.setOutlineThickness(5);
		_HoverRect.setOutlineColor(sf::Color::Black);
	}
	else if (_Action == MenuResult::NextSkin || _Action == MenuResult::PreviousSkin) {
		_HoverRect.setPosition(_Text.getPosition() + sf::Vector2f(0, 15));
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
		//_Text.setStyle(sf::Text::Bold);
	}
	else {
		//_Text.setStyle(sf::Text::Regular);
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
