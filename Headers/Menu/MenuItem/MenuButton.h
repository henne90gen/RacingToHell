#pragma once
#include "Menu/MenuItem/MenuItem.h"
class MenuButton : public MenuItem
{
public:
	MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, std::string text, TextAlignment align);
	~MenuButton();

	void render(sf::RenderWindow& Window);
	MenuResult handleEvent(sf::Event & Event, sf::Vector2f MousePos);

	sf::FloatRect & getRect();
private:
	sf::Text _Text;
	TextAlignment _Alignment;
	sf::RectangleShape _Background;
};

