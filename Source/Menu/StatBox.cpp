#include "stdafx.h"
#include "StatBox.h"

StatBox::StatBox(sf::Vector2f Position)
{
	_Background.setSize(sf::Vector2f(400, 300));
	_Background.setPosition(Position - sf::Vector2f(_Background.getSize().x / 2, 0));
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setOutlineColor(sf::Color::Black);
	_Background.setOutlineThickness(2);
}

StatBox::~StatBox()
{

}

void StatBox::render(sf::RenderWindow& RenderWindow, int SelectedCar)
{
	//Background
	RenderWindow.draw(_Background);

	//Balken
	std::vector<int> Stats = PlayerStats::getPlayerStats(SelectedCar);

	for (unsigned int i = 0; i < 4; i++)
	{

	}
}