//
// Created by henne on 02.02.17.
//

#pragma once

#include "MenuItem.h"
#include <StandardCursor.h>

class Cross {
public:
    Cross() {}

    Cross(sf::Vector2f size, sf::Color color);

    void render(sf::RenderWindow &renderWindow);

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


