// RacingToHell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//New comment

		window.clear();
		window.draw(shape);
		window.display();//ich weiﬂ nicht was ich hier tu.

		//Test‰nderung
	}

	return 0;
}