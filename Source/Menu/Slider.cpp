#include "stdafx.h"
#include "Menu/Slider.h"

Slider::Slider(sf::Vector2f pos, MenuResult action, std::string text, float value, float maxValue) : MenuItem(MenuItems::MSlider, action), _Value(value), _MaxValue(maxValue)
{
	_Line.setFillColor(sf::Color::Black);
	_Line.setSize(sf::Vector2f(200, 5));
	_Line.setPosition(pos);

	_Text.setFont(_Font);
	_Text.setString(text);
	_Text.setPosition(sf::Vector2f(_Line.getPosition().x - _Text.getLocalBounds().width - 10,
		_Line.getPosition().y - _Text.getLocalBounds().height + _Line.getLocalBounds().height / 2.0f));

	_Slider.setFillColor(sf::Color::White);
	_Slider.setSize(sf::Vector2f(10, 30));
	_Slider.setOutlineThickness(1);
	_Slider.setOutlineColor(sf::Color::Black);
	_Slider.setOrigin(sf::Vector2f(_Slider.getLocalBounds().width / 2.0f, _Slider.getLocalBounds().height / 2.0f));
	_Slider.setPosition(sf::Vector2f(_Line.getPosition().x + _Line.getLocalBounds().width * _Value / _MaxValue,
		_Line.getPosition().y + _Line.getLocalBounds().height / 2.0f));

	_BoundingBox.left = _Line.getPosition().x;
	_BoundingBox.top = _Slider.getPosition().y - _Slider.getLocalBounds().height / 2.0f;
	_BoundingBox.width = _Line.getLocalBounds().width;
	_BoundingBox.height = _Slider.getLocalBounds().height;
}

Slider::~Slider()
{
}

void Slider::render(sf::RenderWindow & Window)
{
	Window.draw(_Text);
	Window.draw(_Line);
	Window.draw(_Slider);
}

void Slider::switchHoverState(bool hoverState, bool joystickSelected)
{
	_Hovering = hoverState;
	_JoystickSelected = joystickSelected;

	if ((_Hovering && _Enabled) || (_JoystickSelected && _Enabled)) {
		_Slider.setOutlineThickness(3);
	}
	else if (_Enabled) {
		_Slider.setOutlineThickness(1);
	}
	else {
		//TODO: Add design for disabled slider
	}
}

void Slider::setValue(float value)
{
	_Value = value;
	if (_Value < 0)
		_Value = 0;
	else if (_Value > _MaxValue)
		_Value = _MaxValue;
	_Slider.setPosition(_Line.getPosition().x + _Line.getSize().x * value / _MaxValue, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
}

void Slider::moveSlider(sf::Vector2f newPos)
{
	_Slider.setPosition(newPos.x, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
	_Value = (_Slider.getPosition().x - _Line.getPosition().x) * _MaxValue / _Line.getLocalBounds().width;
}
