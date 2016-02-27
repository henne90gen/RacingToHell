#include "stdafx.h"
#include "Highscore.h"

Highscore::Highscore(sf::Vector2f Position)
{
	_Font.loadFromFile("Resources\\Font\\arial.ttf");
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setSize(sf::Vector2f(450, 500));
	_Background.setPosition(Position);
}

Highscore::~Highscore()
{

}

void Highscore::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Background);
}