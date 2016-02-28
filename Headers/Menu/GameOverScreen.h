#pragma once

#include "Menu/Menu.h"
#include "Textbox.h"
#include "Highscore.h"

class GameOverScreen : public Menu
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);
	void update(int Score, int Level);

	void loadHighScores();

	void playSounds();
	void setVolume(float Volume);

private:
	int _Score, _Level;
	bool _NewHighScore, _ScoreSubmitted;;

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

