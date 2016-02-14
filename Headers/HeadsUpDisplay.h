#pragma once
class HeadsUpDisplay
{
public:
	HeadsUpDisplay();
	~HeadsUpDisplay();

	void render(sf::RenderWindow& Window);
	void update(int Score, int Health, int MaxHealth, int Energy, int MaxEnery);

private:
	sf::Font _Font;
	sf::Texture _BackgroundTexture;
	sf::Sprite _Background;

	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarOutline;
	sf::Text _HealthText;
	sf::Texture _HealthIconTexture;
	sf::Sprite _HealthIconSprite;

	sf::RectangleShape _EnergyBar;
	sf::RectangleShape _EnergyBarOutline;
	sf::Text _EnergyText;
	sf::Texture _EnergyIconTexture;
	sf::Sprite _EnergyIconSprite;

	sf::Text _ScoreText;

	std::string ConvertScore(int Score);
};

