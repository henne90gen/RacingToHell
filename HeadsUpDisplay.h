#pragma once
class HeadsUpDisplay
{
public:
	HeadsUpDisplay();
	~HeadsUpDisplay();

	void render(sf::RenderWindow& Window);
	void update(int Health, int Score, int Energy, int MaxEnery);

private:
	sf::Text _HealthText;
	sf::Text _ScoreText;
	sf::Font _Font;
	sf::RectangleShape _EnergyBar;
	sf::RectangleShape _EnergyOutline;
};

