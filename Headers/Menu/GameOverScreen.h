#pragma once

#include "Menu/Menu.h"
#include "Menu/MenuItem/Textbox.h"
#include "Menu/MenuItem/HighscoreList.h"

class GameOverScreen : public Menu
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window);

	GameState handleEvents(sf::RenderWindow& Window);
	GameState handleMenuItemResult(MenuResult result); 
	
	void update(int Score, int Level);

	void load();

	void playSounds();
	void setVolume(float Volume);

private:
	int _Level;
	bool _ScoreSubmitted;

	//GOT = GameOverText
	sf::Text _GOTLine1;
	sf::Text _GOTLine2;
	sf::Text _GOTLine3;
	sf::Text _GOTLine4;

	HighscoreList _Highscore;

	sf::SoundBuffer _GameOverSoundBuffer;
	sf::Sound _GameOverSound;
	bool _SoundPlayed;
};

