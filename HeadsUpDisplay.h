#pragma once
class HeadsUpDisplay
{
public:
	HeadsUpDisplay();
	~HeadsUpDisplay();

	void render(sf::RenderWindow& Window);
	void update(int Health, int Score, int Energy);

private:
	sf::Text _HealthText;
	sf::Text _ScoreText;
	sf::RectangleShape _EnergyBar;
	sf::RectangleShape _EnergyOutline;
};

