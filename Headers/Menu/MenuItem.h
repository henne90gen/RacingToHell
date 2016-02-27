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
	bool getHoverState() { return _Hovering; }

	void switchHoverState(bool hoverState);
private:
	sf::Text _Text;
	sf::Font _Font;
	sf::RectangleShape _HoverRect;
	MenuResult _Action;
	bool _Hovering;
};

