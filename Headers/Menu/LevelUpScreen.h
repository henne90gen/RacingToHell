#pragma once

class LevelUpScreen
{
public:
	LevelUpScreen();
	~LevelUpScreen();

	void render(sf::RenderWindow& Window);
	bool update();

	void levelUp();

private:
	sf::Font _Font;
	sf::Text _Text;

	sf::RectangleShape _Background;

	sf::Clock _Timer;
	sf::Clock _FadeTimer;

	float _ShowTime, _FadeTime;
};

