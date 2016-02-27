#pragma once
#include "Menu/MenuItem.h"
#include "Menu/MenuResults.h"

class Menu
{
public:
	Menu();
	~Menu();

	virtual void render(sf::RenderWindow& Window) = 0;
	virtual GameState handleEvents(sf::RenderWindow& Window) = 0;

	void checkMouseHover(sf::RenderWindow& Window);

	std::vector<MenuItem*>& getMenuItems() { return _MenuItems; }

protected:
	std::vector<MenuItem*> _MenuItems;
	sf::Font _Font;
	sf::Text _Text;
	sf::Event _Event;
};

