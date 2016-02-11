#pragma once

#include "MenuResults.h"
class GameOverScreen
{
public:
	GameOverScreen();
	~GameOverScreen();

	MenuResult render(sf::RenderWindow& Window, int score);
	MenuResult getMenuResponse(sf::RenderWindow& Window);
	MenuResult handleClick(sf::Vector2f MousePos);

private:
	sf::Font _Font;
	sf::Text _GameOverText;
};

