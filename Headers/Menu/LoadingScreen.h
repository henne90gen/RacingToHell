#pragma once
#include "Menu/Menu.h"

class LoadingScreen : public Menu
{
public:
	LoadingScreen();
	~LoadingScreen();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);
};

