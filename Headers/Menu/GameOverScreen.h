#pragma once
#include "Menu/Menu.h"

class GameOverScreen : public Menu
{
public:
	/*
		After the player dies this screen is shown. It has the HighscoreList on it to add the players score
	*/
	GameOverScreen();
	~GameOverScreen() {}

	/*
		Renders the GameOverScreen to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Handles events for the GameOverScreen
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow& window);
	
	/*
		Handles the result of the different MenuItems that are on the GameOverScreen
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result); 
	
	/*
		Update the GameOverScreen with score and level
		@parame score Score of the player
		@param level Level the player is at
	*/
	void update(int score, int level);

	/*
		Loads the game over sound
	*/
	void load();

	/*
		Plays the game over sound
	*/
	void playSounds();

	/*
		Sets the volume for the game over sound
		@param volume New volume
	*/
	void setVolume(float volume);

private:
	int _Level;
	bool _ScoreSubmitted;

	//GOT = GameOverText
	sf::Text _GOTLine1;
	sf::Text _GOTLine2;
	sf::Text _GOTLine3;

	HighscoreList _Highscore;

	sf::SoundBuffer _GameOverSoundBuffer;
	sf::Sound _GameOverSound;
	bool _SoundPlayed;
};

