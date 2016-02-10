#pragma once
#include "MenuResults.h"

class MenuItem
{
public:
	MenuItem(sf::Vector2f pos, MenuResult action);
	~MenuItem();

	void render(sf::RenderWindow& Window);

private:
	sf::Text _Text;
	sf::Font _Font;
	MenuResult _Action;
};

