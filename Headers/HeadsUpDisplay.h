#pragma once
class HeadsUpDisplay
{
public:
	/*
		Takes care of updating and rendering the HUD with all the vital information that the player needs to play the game.
		Displayed are health energy and score.
	*/
	HeadsUpDisplay();
	~HeadsUpDisplay() {}

	/*
		Renders the HeadsUpDisply to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);
	
	/*
		Updates all the numbers on the HeadsUpDisplay 
		@param score Score the player has earned
		@param health Current health of the player
		@param energy Current energy of the player
	*/
	void update(int score, int health, int energy);

	/*
		@param maxHealth Maximum health the player can have
	*/
	void setMaxHealth(int maxHealth) { _MaxHealth = maxHealth; }

	/*
		@param maxEnergy Maximum energy the player can have
	*/
	void setMaxEnergy(int maxEnergy) { _MaxEnergy = maxEnergy; }

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

	int _MaxHealth, _MaxEnergy;

	std::string ConvertScore(int score);
};

