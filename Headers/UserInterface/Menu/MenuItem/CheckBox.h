//
// Created by henne on 02.02.17.
//

#pragma once


#include "MenuItem.h"

class Cross {
public:
    Cross() {}

    Cross(sf::Vector2f size, sf::Color color) : _Visible(true) {
        sf::Vector2f barSize = sf::Vector2f(size.x * size.x + size.y * size.y, _BarHeight);
        _Left = sf::RectangleShape(barSize);
        _Left.setOrigin(barSize.x / 2, barSize.y / 2);
        _Left.setRotation(45);
        _Left.setFillColor(color);

        _Right = sf::RectangleShape(barSize);
        _Right.setOrigin(barSize.x / 2, barSize.y / 2);
        _Right.setRotation(-45);
        _Right.setFillColor(color);
    }

    void render(sf::RenderWindow &renderWindow) {
        if (_Visible) {
            renderWindow.draw(_Left);
            renderWindow.draw(_Right);
        }
    }

    void setVisible(bool visible) { _Visible = visible; }

    bool isVisible() { return _Visible; }

    void setPosition(sf::Vector2f position) {
        _Left.setPosition(position);
        _Right.setPosition(position);
    }

private:
    bool _Visible;
    sf::RectangleShape _Left, _Right;
    int _BarHeight = 5;
};

class CheckBox : public MenuItem<bool> {
public:
    CheckBox(sf::Vector2f pos, sf::Font &font, std::string text);

    CheckBox() {}

    void render(sf::RenderWindow &renderWindow);

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos);

    sf::FloatRect getRect();

    std::string getText() { return _Text.getString(); }

    bool getValue() { return _Checked; }

    bool getMaxValue() { return true; }

    void setValue(bool newValue) {
        _Checked = newValue;
        _Cross.setVisible(newValue);
    }

private:
    bool _Checked;
    sf::Text _Text;
    sf::RectangleShape _BoxBackground;
    Cross _Cross;
    int _BoxTextPadding = 5;

};


