#pragma once
#include "Menu/MenuItem.h"
class MenuButton : public MenuItem
{
public:
	MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, std::string text, TextAlignment align);
	~MenuButton();

	void render(sf::RenderWindow& Window);

	sf::FloatRect getRect();
	void switchHoverState(bool hoverState, bool joystickSelected);
private:
	sf::Text _Text;
	TextAlignment _Alignment;
	sf::RectangleShape _Background;
};

