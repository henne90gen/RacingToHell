#pragma once

#include "Menu/Menu.h"
#include "Textbox.h"
#include "Highscore.h"

class GameOverScreen : public Menu
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window, int score);
	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);

	void update();

private:
	sf::Font _Font;
	sf::Text _GameOverText;

	bool _IsRendered;

	//GOT = GameOverText
	sf::Text _GOTLine1;
	sf::Text _GOTLine2;
	sf::Text _GOTLine3;
	sf::Text _GOTLine4;

	Textbox* _Textbox;
	Highscore* _Highscore;
};

