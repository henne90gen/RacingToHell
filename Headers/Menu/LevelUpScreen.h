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

private:
	sf::Font _Font;
	sf::Text _Text;

	sf::Texture _Texture;
	sf::Sprite _Sprite;

	sf::RectangleShape _Background;

	sf::Clock _Timer;
	sf::Clock _FadeTimer;

	float _ShowTime, _FadeTime;
};

