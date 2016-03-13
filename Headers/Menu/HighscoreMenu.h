#pragma once
#include "Menu/Menu.h"
#include "Menu/MenuItem/HighscoreList.h"

class HighscoreMenu : public Menu
{
public:
	HighscoreMenu();
	~HighscoreMenu();

	void render(sf::RenderWindow & Window);
	GameState handleEvents(sf::RenderWindow & Window);
	GameState handleMenuItemResult(MenuResult result);

private:
	HighscoreList _List;
};

