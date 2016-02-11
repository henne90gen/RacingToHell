#pragma once
#include "stdafx.h"
#include "MenuItem.h"
#include "MenuResults.h"

class Menu
{
public:
	Menu();
	~Menu();

	MenuResult render(sf::RenderWindow& Window);
	MenuResult getMenuResponse(sf::RenderWindow& Window);
	MenuResult handleClick(sf::Vector2f MousePos);

private:
	sf::Sprite _Background;
	std::vector<MenuItem*> _MenuItems;
};

