#include "stdafx.h"
#include "HeadsUpDisplay.h"


HeadsUpDisplay::HeadsUpDisplay()
{
	sf::Font font;
	font.loadFromFile("Resources/arial.ttf");
	_HealthText.setFont(font);
	_HealthText.setCharacterSize(35);
	_HealthText.setColor(sf::Color::Magenta);
	_HealthText.setPosition(sf::Vector2f(0,0));
	_HealthText.setString("HP: ");

	_ScoreText.setFont(font);
	_ScoreText.setCharacterSize(35);
	_ScoreText.setColor(sf::Color::Magenta);
	_ScoreText.setPosition(sf::Vector2f(0,0));
	_ScoreText.setString("Score: ");
	
	_EnergyBar.setPosition(sf::Vector2f(10, 10));
	_EnergyBar.setFillColor(sf::Color::Green);
	_EnergyBar.setOutlineThickness(0);
	_EnergyBar.setSize(sf::Vector2f(100, 30));

	_EnergyOutline.setPosition(sf::Vector2f(10, 10));
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
	//Health, Score, Energy/Benzin
	//Window.draw(_HealthText);
	//Window.draw(_ScoreText);
	Window.draw(_EnergyOutline);
	Window.draw(_EnergyBar);
}

void HeadsUpDisplay::update(int Health, int Score, int Energy)
{
	_HealthText.setString("HP: " + Health);
	_ScoreText.setString("Score: " + Score);
	_EnergyBar.setSize(sf::Vector2f(Energy, 30));
}
