#pragma once
#include "Menu/MenuItem/MenuItem.h"

class Slider : public MenuItem
{
public:
	Slider(sf::Vector2f pos, MenuResult action, std::string text, float value, float maxValue);
	~Slider();

	void render(sf::RenderWindow& Window);
	MenuResult handleEvent(sf::Event & Event, sf::Vector2f MousePos);

	sf::FloatRect & getRect() { return _BoundingBox; }

	float getValue() { return _Value; }
	void setValue(float value);

	float getMaxValue() { return _MaxValue; }

	void setSlider(float x);
private:
	sf::Text _Text;
	sf::RectangleShape _Slider;
	sf::RectangleShape _Line;
	sf::FloatRect _BoundingBox;

	bool _MouseButtonPressed;

	float _Value, _MaxValue;
};

