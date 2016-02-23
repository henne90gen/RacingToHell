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

	//GOT = GameOverText
	sf::Text _GOTLine1;
	sf::Text _GOTLine2;
	sf::Text _GOTLine3;
};

