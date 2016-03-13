#pragma once

class StatBox
{
public:
	StatBox(sf::Vector2f Position);
	~StatBox();

	void render(sf::RenderWindow& RenderWindow, int SelectedCar);
private:
	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _Headline;
	sf::Texture _StatBarTexture;
};