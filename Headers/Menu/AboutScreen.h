#pragma once
#include "Menu\Menu.h"

class AboutScreen : public Menu
{
public:
	AboutScreen();
	~AboutScreen() {}

	void render(sf::RenderWindow& window);
	GameState handleEvents(sf::RenderWindow & window);
	GameState handleMenuItemResult(MenuResult result);

private:
	sf::Text _LevelUp, _Music, _Cars, _Mech;
};

