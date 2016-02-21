#pragma once

#include "MenuResults.h"

class GameOverScreen
{
public:
	GameOverScreen();
	~GameOverScreen();

	void render(sf::RenderWindow& Window, int score);
	void playSounds();

private:
	sf::Font _Font;
	sf::Text _GameOverText;

	sf::SoundBuffer _GameOverSoundBuffer;
	sf::Sound _GameOverSound;
};

