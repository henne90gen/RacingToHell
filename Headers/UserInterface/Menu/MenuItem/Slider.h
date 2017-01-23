#pragma once

#include "MenuItem.h"

class Slider : public MenuItem {
public:
    /**
        Slider element that can be placed in a menu to change numeric values
        Range of slider is minValue to maxValue
        @param pos Position of the Slider
        @param action Action the Slider is associated with
        @param text Text to the left of the Slider
        @param minValue Minimum value of the slider
        @param maxValue Maximum value of the slider
    */
    Slider(sf::Vector2f pos, MenuResult action, std::string text, float minValue, float maxValue);

    ~Slider() {}

    /*
        See MenuItem for details
    */
    void render(sf::RenderWindow &Window);

    /*
        See MenuItem for details
    */
    MenuResult handleEvent(sf::Event &Event, sf::Vector2f MousePos);

    /*
        See MenuItem for details
    */
    sf::FloatRect getRect() { return _BoundingBox; }

    /*
        @return float Value of the Slider
    */
    float getValue() { return _Value; }

    /*
        @param value New value for the Slider
        @return bool True if the value was set successfully
    */
    void setValue(float value);

    /*
        @return float Maximum value of the Slider
    */
    float getMaxValue() { return _MaxValue; }

    float getMinValue() { return _MinValue; }

    /*
        Sets the Slider to a specific x coordinate relativ to the screen
        @param x New x coordinate for the Slider
    */
    void setSlider(float x);

    /*
        @return std::string Text that is being displayed in front of the slider
    */
    std::string getText() { return _Text.getString(); }

private:
    sf::Text _Text;
    sf::RectangleShape _Slider, _Line, _TextBackground;
    sf::FloatRect _BoundingBox;

    bool _MouseButtonPressed;

    float _Value, _MinValue, _MaxValue;
};
