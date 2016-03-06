#pragma once
#include "Menu/MenuItem.h"

class Slider : public MenuItem
{
public:
	Slider(sf::Vector2f pos, MenuResult action, std::string text, float value, float maxValue);
	~Slider();

	void render(sf::RenderWindow& Window);

	sf::FloatRect getRect() { return _BoundingBox; }
	void switchHoverState(bool hoverState, bool joystickSelected);

	float getValue() { return _Value; }
	void setValue(float value);

	float getMaxValue() { return _MaxValue; }

	void moveSlider(sf::Vector2f newPos);
private:
	sf::Text _Text;
	sf::RectangleShape _Slider;
	sf::RectangleShape _Line;
	sf::FloatRect _BoundingBox;

	float _Value, _MaxValue;
};

