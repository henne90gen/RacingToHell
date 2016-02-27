#pragma once

#include "Textbox.h"
#include "Highscore.h"

class GameOverScreen
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window, int score);
	void update();
	void handleEvent(sf::Event& Event);
private:
	sf::Font _Font;
	sf::Text _GameOverText;

	//GOT = GameOverText
	sf::Text _GOTLine1;
	sf::Text _GOTLine2;
	sf::Text _GOTLine3;
	sf::Text _GOTLine4;

	Textbox* _Textbox;
	Highscore* _Highscore;
};

