#pragma once
#include "MenuResults.h"

class MenuItem
{
public:
	MenuItem(sf::Vector2f pos, MenuResult action);
	~MenuItem();

	void render(sf::RenderWindow& Window);

	sf::FloatRect getRect();
	MenuResult getAction() { return _Action; }
private:
	sf::Text _Text;
	sf::Font _Font;
	MenuResult _Action;
};

