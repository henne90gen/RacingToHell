#include "stdafx.h"
#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay()
{
	//Font
	_Font.loadFromFile("Resources/Font/arial.ttf");

	//Displays the current level
	_LevelText.setFont(_Font);
	_LevelText.setPosition(sf::Vector2f(0, 10));
	_LevelText.setColor(sf::Color::White);

	//Progressbar
	_ProgressBar.setFillColor(sf::Color(255, 255, 255, 100));
	_ProgressBar.setSize(sf::Vector2f(SCREENWIDTH, 10));
	_ProgressBar.setPosition(0, 0);

	//Progressbar outline
	_ProgressBarOutline.setFillColor(sf::Color::Transparent);
	_ProgressBarOutline.setOutlineColor(sf::Color(20, 0, 0));
	_ProgressBarOutline.setOutlineThickness(1);
	_ProgressBarOutline.setSize(_ProgressBar.getSize());

	//Background
	_BackgroundTexture.loadFromFile("Resources/Texture/HUD/HUD_Background.png");
	_Background.setTexture(_BackgroundTexture);
	_Background.setPosition(sf::Vector2f(0, SCREENHEIGHT - _Background.getLocalBounds().height));

	//Healthbar
	_HealthBar.setPosition(sf::Vector2f(_Background.getGlobalBounds().left + 10, SCREENHEIGHT - _Background.getLocalBounds().height + 5));
	_HealthBar.setSize(sf::Vector2f(150, 30));
	_HealthBar.setFillColor(sf::Color(175, 0, 0));

	//Healthbar Outline
	_HealthBarOutline.setPosition(_HealthBar.getPosition());
	_HealthBarOutline.setSize(_HealthBar.getSize());
	_HealthBarOutline.setFillColor(sf::Color::Transparent);
	_HealthBarOutline.setOutlineColor(sf::Color::Black);
	_HealthBarOutline.setOutlineThickness(2);

	//Health Text
	_HealthText.setFont(_Font);
	_HealthText.setCharacterSize(20);
	_HealthText.setColor(sf::Color::White);

	//Health Icon
	_HealthIconTexture.loadFromFile("Resources/Texture/HUD/HUD_HealthIcon.png");
	_HealthIconSprite.setTexture(_HealthIconTexture);
	_HealthIconSprite.setPosition(sf::Vector2f(_HealthBarOutline.getGlobalBounds().left + 6, _HealthBarOutline.getGlobalBounds().top + 6));

	//Energybar
	_EnergyBar.setPosition(sf::Vector2f(_HealthBarOutline.getGlobalBounds().left + _HealthBarOutline.getLocalBounds().width + 20, _HealthBarOutline.getGlobalBounds().top + 2));
	_EnergyBar.setSize(sf::Vector2f(150, 30));
	_EnergyBar.setFillColor(sf::Color(125, 0, 175));

	//Energybar Outline
	_EnergyBarOutline.setPosition(_EnergyBar.getPosition());
	_EnergyBarOutline.setSize(_EnergyBar.getSize());
	_EnergyBarOutline.setFillColor(sf::Color::Transparent);
	_EnergyBarOutline.setOutlineColor(sf::Color::Black);
	_EnergyBarOutline.setOutlineThickness(2);

	//Enrgy Text
	_EnergyText.setFont(_Font);
	_EnergyText.setCharacterSize(20);
	_EnergyText.setColor(sf::Color::White);

	//Energy Icon
	_EnergyIconTexture.loadFromFile("Resources/Texture/HUD/HUD_EnergyIcon.png");
	_EnergyIconSprite.setTexture(_EnergyIconTexture);
	_EnergyIconSprite.setPosition(sf::Vector2f(_EnergyBarOutline.getGlobalBounds().left + 6, _EnergyBarOutline.getGlobalBounds().top + 6));

	//Score
	_ScoreText.setFont(_Font);
	_ScoreText.setCharacterSize(30);
	_ScoreText.setColor(sf::Color::White);
	_ScoreText.setPosition(sf::Vector2f(350, SCREENHEIGHT - _Background.getLocalBounds().height));
	_ScoreText.setString("Score: 00000000");
}

void HeadsUpDisplay::render(sf::RenderWindow & window)
{
	window.draw(_LevelText);

	window.draw(_ProgressBar);
	window.draw(_ProgressBarOutline);

	window.draw(_Background);

	window.draw(_HealthBar);
	window.draw(_HealthBarOutline);
	window.draw(_HealthText);
	window.draw(_HealthIconSprite);

	window.draw(_EnergyBar);
	window.draw(_EnergyBarOutline);
	window.draw(_EnergyText);
	window.draw(_EnergyIconSprite);

	window.draw(_ScoreText);
}

void HeadsUpDisplay::update(int score, int health, int energy, int level, float levelTime, GameMode mode)
{
	_LevelText.setString("Level: " + std::to_string(level));

	_ProgressBar.setSize(sf::Vector2f(levelTime * SCREENWIDTH / _TotalLevelTime, 10));

	_HealthBar.setSize(sf::Vector2f((float)health / (float)_MaxHealth * 150, 30));
	_HealthText.setPosition(sf::Vector2f(_HealthBarOutline.getGlobalBounds().left + _HealthBarOutline.getLocalBounds().width - _HealthText.getLocalBounds().width - 7, _HealthBarOutline.getGlobalBounds().top + 3));

	if (mode == GameMode::Invincible) {
		_HealthText.setString(L"∞");
		_HealthText.setCharacterSize(40);
		_HealthText.setPosition(_HealthText.getPosition() - sf::Vector2f(5, 12));
	} else {
		_HealthText.setString(std::to_string(health) + "/" + std::to_string(_MaxHealth));
		_HealthText.setCharacterSize(20);
	}

	if (health <= 10) {
		_HealthText.setColor(sf::Color(255, 75, 75));
	} else {
		_HealthText.setColor(sf::Color::White);
	}

	_EnergyBar.setSize(sf::Vector2f((float)energy / (float)_MaxEnergy * 150, 30));
	_EnergyText.setPosition(sf::Vector2f(_EnergyBarOutline.getGlobalBounds().left + _EnergyBarOutline.getLocalBounds().width - _EnergyText.getLocalBounds().width - 7, _EnergyBarOutline.getGlobalBounds().top + 3));

	if (mode == GameMode::InfEnergy) {
		_EnergyText.setString(L"∞");
		_EnergyText.setCharacterSize(40);
		_EnergyText.setPosition(sf::Vector2f(_EnergyText.getPosition() - sf::Vector2f(5, 12)));
	} else {
		_EnergyText.setString(std::to_string(energy) + "/" + std::to_string(_MaxEnergy));
	}

	if (energy <= 10) {
		_EnergyText.setColor(sf::Color(255, 75, 75));
	} else {
		_EnergyText.setColor(sf::Color::White);
	}

	_ScoreText.setString("Score: " + ConvertScore(score));
}

std::string HeadsUpDisplay::ConvertScore(int score)
{
	std::string ScoreString;
	int ScoreLength = std::to_string(score).length();

	if (ScoreLength < 7)
	{
		for (unsigned int i = 0; i < 8 - ScoreLength; i++)
		{
			ScoreString += "0";
		}

		ScoreString += std::to_string(score);
	}
	else
	{
		ScoreString = std::to_string(score);
	}

	return ScoreString;
}
