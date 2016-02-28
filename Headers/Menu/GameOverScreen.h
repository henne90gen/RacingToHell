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

	void loadHighScores();

	void playSounds();
	void setVolume(float Volume);

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

	sf::SoundBuffer _GameOverSoundBuffer;
	sf::Sound _GameOverSound;
	bool _SoundPlayed;
};

