#include "stdafx.h"
#include "HeadsUpDisplay.h"


HeadsUpDisplay::HeadsUpDisplay()
{
	//Font
	_Font.loadFromFile("Resources/Font/arial.ttf");

	//Background
	_BackgroundTexture.loadFromFile("Resources/Texture/HUD/HUD_Background2.png");
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
	_ScoreText.setPosition(sf::Vector2f(370, SCREENHEIGHT - _Background.getLocalBounds().height));
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
	_HealthText.setPosition(sf::Vector2f(	_HealthBarOutline.getGlobalBounds().left + _HealthBarOutline.getLocalBounds().width - _HealthText.getLocalBounds().width - 7, 
											_HealthBarOutline.getGlobalBounds().top + 3));

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
	_EnergyText.setPosition(sf::Vector2f(_EnergyBarOutline.getGlobalBounds().left + _EnergyBarOutline.getLocalBounds().width - _EnergyText.getLocalBounds().width - 7,
		_EnergyBarOutline.getGlobalBounds().top + 3));

	if (Energy <= 10)
	{
		_EnergyText.setColor(sf::Color(255, 75, 75));
	}
	else
	{
		_EnergyText.setColor(sf::Color::White);
	}

	_ScoreText.setString("Score: " + ConvertScore(Score));
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