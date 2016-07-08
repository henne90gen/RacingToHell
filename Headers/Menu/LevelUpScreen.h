#pragma once

class LevelUpScreen
{
public:
	/*
		Shows changes that are going to be made to the difficulty
	*/
	LevelUpScreen();
	~LevelUpScreen() {}

	/*
		Renders the HighscoreMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Updates the LevelUpScreen and all the animations
		@return bool True when the animations are done
	*/
	bool update();

	/*
		Sets up the LevelUpScreen for the next level
	*/
	void levelUp();

	/*
		Plays the level up sound effect
	*/
	void playSound();

	/*
		Sets the volume for the level up sound effect
		@param volume New volume level
	*/
	void setVolume(float volume) { _Sound.setVolume(volume); }
private:
	sf::Font _Font;
	sf::Text _Text;

	sf::Texture _FasterTexture;
	sf::Sprite _Faster;
	sf::Texture _StrongerTexture;
	sf::Sprite _Stronger;

	sf::RectangleShape _Background;

	sf::SoundBuffer _SoundBuffer;
	sf::Sound _Sound;
	bool _SoundPlayed;
	float _SoundDelay;

	sf::Clock _Timer;
	sf::Clock _FadeTimer;

	float _ShowTime, _FadeTime;
};

