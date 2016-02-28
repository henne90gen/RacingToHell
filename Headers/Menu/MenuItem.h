#pragma once
#include "MenuResults.h"
#include "AlignText.h"

class MenuItem
{
public:
	MenuItem(sf::Vector2f pos, sf::Vector2f size, MenuResult action);
	~MenuItem();

	void render(sf::RenderWindow& Window);

	sf::FloatRect getRect();
	MenuResult getAction() { return _Action; }
	bool getHoverState() { return _Hovering; }

	void switchHoverState(bool hoverState);
private:
	sf::Text _Text;
	sf::Font _Font;
	sf::RectangleShape _Background;

	MenuResult _Action;
	TextAlignment _Alignment;

	bool _Hovering;
};

