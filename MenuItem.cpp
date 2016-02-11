#include "stdafx.h"
#include "MenuItem.h"


MenuItem::MenuItem(sf::Vector2f pos, MenuResult action) : _Action(action)
{
	_Font.loadFromFile("Resources/arial.ttf");
	
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setColor(sf::Color::Blue);
	_Text.setCharacterSize(50);

	switch (action) {
	case MenuResult::Resume:
		_Text.setString("Play");
		break;
	case MenuResult::PreviousSkin:
		_Text.setString("Previous");
		break;
	case MenuResult::NextSkin:
		_Text.setString("Next");
		break;
	case MenuResult::Exit:
		_Text.setString("Exit");
		break;
	}

	//Initializing rectangle that will be shown at hover-over
	//Position is adjusted by 10 down and size is increased by 10 horizontally and 7 vertically
	_HoverRect.setPosition(pos + sf::Vector2f(0, 10));
	_HoverRect.setSize(sf::Vector2f(_Text.getLocalBounds().width + 10, _Text.getLocalBounds().height + 7));
	_HoverRect.setFillColor(sf::Color::Green);
	_HoverRect.setOutlineThickness(5);
	_HoverRect.setOutlineColor(sf::Color::Black);
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

void MenuItem::switchHoverState(bool hoverState, sf::RenderWindow & Window)
{
	_Hovering = hoverState;
	render(Window);
}
