#include "stdafx.h"
#include "Menu/MenuItem\Slider.h"

Slider::Slider(sf::Vector2f pos, MenuResult action, std::string text, float value, float maxValue) : MenuItem(MenuItemType::MSlider, action), _Value(value), _MaxValue(maxValue)
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

void Slider::render(sf::RenderWindow & Window)
{
	if ((_Hovering && _Enabled) || (_Focused && _Enabled)) {
		_Slider.setOutlineColor(sf::Color::Black);
		_Slider.setOutlineThickness(3);
		_Slider.setFillColor(sf::Color::White);
	}
	else if (_Enabled) {
		_Slider.setOutlineColor(sf::Color::Black);
		_Slider.setOutlineThickness(1);
		_Slider.setFillColor(sf::Color::White);
	}
	else {
		_Slider.setOutlineThickness(1);
		_Slider.setFillColor(sf::Color(170, 161, 161));
		_Slider.setOutlineColor(sf::Color(80, 73, 73));
	}

	Window.draw(_Text);
	Window.draw(_Line);
	Window.draw(_Slider);
}

MenuResult Slider::handleEvent(sf::Event & Event, sf::Vector2f MousePos)
{
	if (_Enabled) {
		if (Event.type == sf::Event::MouseButtonPressed) {
			if (MousePos.y > getRect().top && MousePos.y < getRect().top + getRect().height && MousePos.x > getRect().left && MousePos.x < getRect().left + getRect().width)
			{
				setSlider(MousePos.x);
				_MouseButtonPressed = true;
			}
		}
		else if (Event.type == sf::Event::MouseButtonReleased) {
			_MouseButtonPressed = false;
		}
		else if (Event.type == sf::Event::MouseMoved) {
			if (MousePos.y > getRect().top && MousePos.y < getRect().top + getRect().height && MousePos.x > getRect().left && MousePos.x < getRect().left + getRect().width) {
				_Hovering = true;
			}
			else {
				_Hovering = false;
			}
			if (_MouseButtonPressed) {
				_Hovering = true;
				setSlider(MousePos.x);
			}
			_Focused = false;
		}
	}
	return _Action;
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

void Slider::setSlider(float x)
{
	if (x > _Line.getPosition().x && x < _Line.getPosition().x + _Line.getLocalBounds().width) {
		_Slider.setPosition(x, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
	}
	else {
		if (x < _Line.getPosition().x) {
			_Slider.setPosition(_Line.getPosition().x, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
		}
		else if (x > _Line.getPosition().x + _Line.getLocalBounds().width) {
			_Slider.setPosition(_Line.getPosition().x + _Line.getLocalBounds().width, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
		}
	}
	_Value = (_Slider.getPosition().x - _Line.getPosition().x) * _MaxValue / _Line.getLocalBounds().width;
}
