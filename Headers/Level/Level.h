#pragma once

class Level 
{
public:
	/*
		Manager class for the level, draws background and plays music
	*/
	Level();
	~Level() {}

	/*
		Renders the level background to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Updates the Level with the given frame time
		@param frameTime Time that has passed since the last update
		@param gameState State of the game
		@return bool True if 'level up' has happened
	*/
	bool update(float frameTime, GameState gameState);
	
	/*
		Plays the level background music
	*/
	void playMusic();

	/*
		Stops the Level background music
	*/
	void stopMusic();

	/*
		Pauses the Level background music
	*/
	void pauseMusic();

	/*
		Sets the volume to the given value
		@param volume New Volume value
	*/
	void setVolume(float volume);

	/*
		Resets the Level to a state where a new game can be started
	*/
	void resetLevel();

	/*
		Tells the Level to increase the difficulty and to change the background
	*/
	void levelUp();

	/*
		Loads all textures and music
	*/
	void load();

	/*
		@return int Difficulty of the Level
	*/
	int getDifficulty() { return _Difficulty; }

	/*
		@return int Speed of the road
	*/
	int getRoadSpeed() { return (100 * _Difficulty + 100); }

	/*
		Resets the internal timer of the Level
	*/
	void resetTimer() { _Timer.restart(); }

private:
	std::vector<std::shared_ptr<sf::Texture>> _Textures;
	sf::Sprite _Sprite;

	std::vector<std::shared_ptr<sf::SoundBuffer>> _MusicBuffers;
	sf::Sound _Music;

	sf::Clock _Timer;

	int _Difficulty;
	float _LevelUp;
};