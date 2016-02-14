#pragma once
#include "stdafx.h"
#include "MenuItem.h"
#include "MenuResults.h"

class Menu
{
public:
	Menu();
	~Menu();

	void render(sf::RenderWindow& Window);
	void update(float FrameTime);

	void checkMouseHover(sf::RenderWindow& Window);

	std::vector<MenuItem*>& getMenuItems() { return _MenuItems; }

private:
	sf::Sprite _CarSkin;
	std::vector<MenuItem*> _MenuItems;
};

