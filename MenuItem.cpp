#include "stdafx.h"
#include "MenuItem.h"


MenuItem::MenuItem(sf::Vector2f pos, MenuResult action) : _Action(action)
{
	_Font.loadFromFile("Resources/arial.ttf");
	_Text.setFont(_Font);
	_Text.setPosition(pos);
	_Text.setCharacterSize(50);
	switch (action) {
	case MenuResult::Resume:
		_Text.setString("Resume");
		break;
	case MenuResult::Exit:
		_Text.setString("Exit");
		break;
	}
}

MenuItem::~MenuItem()
{
}

void MenuItem::render(sf::RenderWindow & Window)
{
	Window.draw(_Text);
}
