#pragma once
#include "Menu\Menu.h"

class PauseMenu : public Menu
{
public:
	PauseMenu();
	~PauseMenu();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);
	GameState handleMenuItemResult(MenuResult result);
};

