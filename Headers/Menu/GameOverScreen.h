#pragma once

#include "MenuResults.h"

class GameOverScreen
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window, int score);

private:
	sf::Font _Font;
	sf::Text _GameOverText;
};

