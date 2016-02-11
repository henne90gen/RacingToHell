#include "stdafx.h"
#include "HeadsUpDisplay.h"


HeadsUpDisplay::HeadsUpDisplay()
{
	_Font.loadFromFile("Resources/arial.ttf");
	_HealthText.setFont(_Font);
	_HealthText.setCharacterSize(35);
	_HealthText.setColor(sf::Color::Magenta);
	_HealthText.setPosition(sf::Vector2f(0,0));
	_HealthText.setString("HP: ");

	_ScoreText.setFont(_Font);
	_ScoreText.setCharacterSize(35);
	_ScoreText.setColor(sf::Color::Magenta);
	_ScoreText.setPosition(sf::Vector2f(150,0));
	_ScoreText.setString("Score: ");
	
	_EnergyBar.setPosition(sf::Vector2f(350, 10));
	_EnergyBar.setFillColor(sf::Color::Green);
	_EnergyBar.setOutlineThickness(0);
	_EnergyBar.setSize(sf::Vector2f(100, 30));

	_EnergyOutline.setPosition(sf::Vector2f(350, 10));
	_EnergyOutline.setFillColor(sf::Color::Transparent);
	_EnergyOutline.setOutlineColor(sf::Color::Black);
	_EnergyOutline.setOutlineThickness(3);
	_EnergyOutline.setSize(sf::Vector2f(100, 30));
}


HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::render(sf::RenderWindow & Window)
{
	Window.draw(_HealthText);
	Window.draw(_ScoreText);
	Window.draw(_EnergyOutline);
	Window.draw(_EnergyBar);
}

void HeadsUpDisplay::update(int Health, int Score, int Energy)
{
	_HealthText.setString("HP: " + std::to_string(Health));
	_ScoreText.setString("Score: " + std::to_string(Score));
	_EnergyBar.setSize(sf::Vector2f(Energy, 30));
}
