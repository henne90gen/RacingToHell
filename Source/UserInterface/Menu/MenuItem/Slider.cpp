#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/Slider.h"

Slider::Slider(sf::Vector2f pos, sf::Font &font, std::string text, float minValue, float maxValue)
        : MenuItem(
        MenuItemType::MSlider, font), _MinValue(minValue), _MaxValue(maxValue) {

    _Line.setFillColor(sf::Color::Black);
    _Line.setSize(sf::Vector2f(200, 5));
    _Line.setPosition(pos);

    _Text.setFont(font);
    _Text.setString(text);
    _Text.setPosition(sf::Vector2f(_Line.getPosition().x - _Text.getLocalBounds().width - 30,
                                   _Line.getPosition().y - _Text.getLocalBounds().height +
                                   _Line.getLocalBounds().height / 2.0f));

    _TextBackground.setPosition(sf::Vector2f(_Text.getPosition().x - 10, _Text.getPosition().y - 3));
    _TextBackground.setSize(sf::Vector2f(_Text.getLocalBounds().width + 20, _Text.getLocalBounds().height + 20));
    _TextBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _TextBackground.setOutlineThickness(1);
    _TextBackground.setOutlineColor(sf::Color::Black);

    _Slider.setFillColor(sf::Color::White);
    _Slider.setSize(sf::Vector2f(10, 30));
    _Slider.setOutlineThickness(1);
    _Slider.setOutlineColor(sf::Color::Black);
    _Slider.setOrigin(sf::Vector2f(_Slider.getLocalBounds().width / 2.0f, _Slider.getLocalBounds().height / 2.0f));
    _Slider.setPosition(sf::Vector2f(_Line.getPosition().x + _Line.getLocalBounds().width * _Value / _MaxValue,
                                     _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f));

    _BoundingBox.left = _Line.getPosition().x - _Slider.getLocalBounds().width;
    _BoundingBox.top = _Slider.getPosition().y - _Slider.getLocalBounds().height / 2.0f;
    _BoundingBox.width = _Line.getLocalBounds().width + _Slider.getLocalBounds().width * 1.3f;
    _BoundingBox.height = _Slider.getLocalBounds().height;
}

void Slider::render(sf::RenderWindow &renderWindow) {
    if ((_Hovering && _Enabled) || (_Focused && _Enabled)) {
        _Slider.setOutlineColor(sf::Color::Black);
        _Slider.setOutlineThickness(3);
        _Slider.setFillColor(sf::Color::White);
    } else if (_Enabled) {
        _Slider.setOutlineColor(sf::Color::Black);
        _Slider.setOutlineThickness(1);
        _Slider.setFillColor(sf::Color::White);
    } else {
        _Slider.setOutlineThickness(1);
        _Slider.setFillColor(sf::Color(170, 161, 161));
        _Slider.setOutlineColor(sf::Color(80, 73, 73));
    }

    renderWindow.draw(_TextBackground);
    renderWindow.draw(_Text);
    renderWindow.draw(_Line);
    renderWindow.draw(_Slider);
}

bool Slider::handleEvent(sf::Event &Event, sf::Vector2f MousePos) {
    if (_Enabled) {
        if (Event.type == sf::Event::MouseButtonPressed) {
            if (rh::pointInRectangle(getRect(), MousePos)) {
                setSlider(MousePos.x);
                _MouseButtonPressed = true;
                return true;
            }
        } else if (Event.type == sf::Event::MouseButtonReleased) {
            _MouseButtonPressed = false;
        } else if (Event.type == sf::Event::MouseMoved) {
            _Hovering = rh::pointInRectangle(getRect(), MousePos);
            if (_MouseButtonPressed) {
                _Hovering = true;
                setSlider(MousePos.x);
                return true;
            }
            _Focused = false;
        }
    }
    return false;
}

void Slider::setValue(float value) {
    _Value = value;
    if (_Value < _MinValue) {
        _Value = _MinValue;
    } else if (_Value > _MaxValue) {
        _Value = _MaxValue;
    }
    _Slider.setPosition(
            _Line.getPosition().x + _Line.getLocalBounds().width * (_Value - _MinValue) / (_MaxValue - _MinValue),
            _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
}

void Slider::setSlider(float x) {
    if (x > _Line.getPosition().x && x < _Line.getPosition().x + _Line.getLocalBounds().width) {
        _Slider.setPosition(x, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
    } else {
        if (x < _Line.getPosition().x) {
            _Slider.setPosition(_Line.getPosition().x, _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
        } else if (x > _Line.getPosition().x + _Line.getLocalBounds().width) {
            _Slider.setPosition(_Line.getPosition().x + _Line.getLocalBounds().width,
                                _Line.getPosition().y + _Line.getLocalBounds().height / 2.0f);
        }
    }

    _Value = (_Slider.getPosition().x - _Line.getPosition().x) * (_MaxValue - _MinValue) / _Line.getLocalBounds().width;
    _Value += _MinValue;
}
