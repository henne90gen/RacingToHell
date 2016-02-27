#pragma once

#include "Menu/Menu.h"

class GameOverScreen : public Menu
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window, int score);
	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);

private:
	sf::Font _Font;
	sf::Text _GameOverText;
};

