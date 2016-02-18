#include "stdafx.h"
#include "HeadsUpDisplay.h"


HeadsUpDisplay::HeadsUpDisplay()
{
	//Font
	_Font.loadFromFile("Resources/Font/arial.ttf");

	//Background
	_BackgroundTexture.loadFromFile("Resources/Texture/HUD/HUD_Background.png");
	_Background.setTexture(_BackgroundTexture);
	_Background.setPosition(sf::Vector2f(0, 0));

	//Healthbar
	_HealthBar.setPosition(sf::Vector2f(10, 5));
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
	_HealthText.setPosition(sf::Vector2f(100, 8));

	//Health Icon
	_HealthIconTexture.loadFromFile("Resources/Texture/HUD/HUD_HealthIcon.png");
	_HealthIconSprite.setTexture(_HealthIconTexture);
	_HealthIconSprite.setPosition(sf::Vector2f(15, 10));

	//Energybar
	_EnergyBar.setPosition(sf::Vector2f(180, 5));
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
	_EnergyIconSprite.setPosition(sf::Vector2f(185, 10));

	//Score
	_ScoreText.setFont(_Font);
	_ScoreText.setCharacterSize(30);
	_ScoreText.setColor(sf::Color::White);
	_ScoreText.setPosition(sf::Vector2f(370,0));
	_ScoreText.setString("Score: 0000000");
	

}


HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::render(sf::RenderWindow & Window)
{
	Window.draw(_Background);

	Window.draw(_HealthBar);
	Window.draw(_HealthBarOutline);
	Window.draw(_HealthText);
	Window.draw(_HealthIconSprite);

	Window.draw(_EnergyBar);
	Window.draw(_EnergyBarOutline);
	Window.draw(_EnergyText);
	Window.draw(_EnergyIconSprite);

	Window.draw(_ScoreText);
}

void HeadsUpDisplay::update(int Score, int Health, int MaxHealth, int Energy, int MaxEnergy)
{
	_HealthBar.setSize(sf::Vector2f((float)Health / (float)MaxHealth * 150, 30));
	_HealthText.setString(std::to_string(Health) + "/" + std::to_string(MaxHealth));
	_HealthText.setPosition(sf::Vector2f(155 - _HealthText.getLocalBounds().width, 8));

	if (Health <= 10)
	{
		_HealthText.setColor(sf::Color(255, 75, 75));
	}
	else
	{
		_HealthText.setColor(sf::Color::White);
	}

	_EnergyBar.setSize(sf::Vector2f((float)Energy / (float)MaxEnergy * 150, 30));
	_EnergyText.setString(std::to_string(Energy) + "/" + std::to_string(MaxEnergy));
	_EnergyText.setPosition(sf::Vector2f(325 - _EnergyText.getLocalBounds().width, 8));

	if (Energy <= 10)
	{
		_EnergyText.setColor(sf::Color(255, 75, 75));
	}
	else
	{
		_EnergyText.setColor(sf::Color::White);
	}

	_ScoreText.setString("Score: " + ConvertScore(Score));
	//
}

std::string HeadsUpDisplay::ConvertScore(int Score)
{
	std::string ScoreString;
	int ScoreLength = std::to_string(Score).length();

	if (ScoreLength < 7)
	{
		for (unsigned int i = 0; i < 7 - ScoreLength; i++)
		{
			ScoreString += "0";
		}

		ScoreString += std::to_string(Score);
	}
	else
	{
		ScoreString = std::to_string(Score);
	}

	return ScoreString;
}