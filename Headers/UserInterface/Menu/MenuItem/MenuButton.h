#pragma once

#include "MenuItem.h"

class MenuButton : public MenuItem<float> {
public:
    /*
        Button element that can be placed in a menu
        @param pos Position of the Button on the screen
        @param size Size of the Button
        @param action Action the Button is associated with
        @param text Text that is going to be on the Button
        @param align Alignment of the text on the Button
    */
    MenuButton(sf::Vector2f pos, sf::Vector2f size, sf::Font &font, std::string text, TextAlignment align);

    MenuButton() {}

    ~MenuButton() {}

    void render(sf::RenderWindow &renderWindow);

    bool handleEvent(sf::Event &event, sf::Vector2f mousePos);

    sf::FloatRect getRect();

    /**
     * @return std::string Text of the button
     */
    std::string getText() { return _Text.getString(); }

    /*
        @param text new text
    */
    void setText(std::string text);

    /*
        Button doesn't have a value that could be changed
        @return bool False
    */
    void setValue(float x) {}

    /*
        Button doesn't have a value that could be returned
        @return float 0.0f
    */
    float getValue() { return 0.0f; }

    /*
        Button doesn't have a maximum value that could be returned
        @return float 0.0f
    */
    float getMaxValue() { return 0.0f; }

private:
    sf::Text _Text;
    TextAlignment _Alignment;
    sf::RectangleShape _Background;
};

