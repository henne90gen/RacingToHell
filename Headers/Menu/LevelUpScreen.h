#pragma once

class LevelUpScreen
{
public:
	LevelUpScreen();
	~LevelUpScreen();

	void render(sf::RenderWindow& Window);
	bool update();

private:
	sf::Font _Font;
	sf::Text _Text;

	sf::Clock _Timer;
};

