#pragma once
#include "stdafx.h"
#include "MenuItem.h"
#include "MenuResults.h"

class Menu
{
public:
	Menu();
	~Menu();

	MenuResult render(sf::RenderWindow& Window, sf::Sprite carSkin);
	MenuResult getMenuResponse(sf::RenderWindow& Window);
	MenuResult handleClick(sf::Vector2f MousePos);

	void checkMouseHover(sf::RenderWindow& Window);

private:
	sf::Sprite _CarSkin;
	std::vector<MenuItem*> _MenuItems;
};

