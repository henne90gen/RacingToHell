#pragma once

#include "MenuItem.h"

class ComboBox : public MenuItem {
public:
    ComboBox(sf::Vector2f pos, std::vector<std::string> itemStrings, std::vector<float> itemValues, MenuResult action,
             sf::Font &font, int width = 200, int height = 50);

    virtual ~ComboBox() {};

    /*
        See MenuItem for details
    */
    void render(sf::RenderWindow &renderWindow);

    /*
        See MenuItem for details
    */
    MenuResult handleEvent(sf::Event &newEvent, sf::Vector2f mousePos);

    /*
        See MenuItem for details
    */
    sf::FloatRect getRect();

    /*
        @return std::string Text of the Button
    */
    std::string getText() { return _ItemStrings[_CurrentItem]; }

    /*
        set currentItem
        @return bool False
    */
    void setValue(float x);

    /*
        return current Item
        @return float 0.0f
    */
    float getValue() { return _CurrentItem; }

    /*
        Combo doesn't have a maximum value that could be returned
        @return float 0.0f
    */
    float getMaxValue() { return 0.0f; }

private:
    std::vector<std::string> _ItemStrings;
    std::vector<float> _ItemValues;

    sf::RectangleShape _ButtonLeft, _ButtonRight, _Body;
    sf::Vector2f _Position;
    sf::Texture _ArrowTexture;
    sf::Sprite _LeftArrowSprite, _RightArrowSprite;
    sf::Text _Text;
    sf::Color _BgColorEnabled, _BgColorHovering, _BgColorDisabled;
    sf::Color _FgColorEnabled, _FgColorDisabled;

    bool _HoveringLeft, _HoveringRight;
    int _Height, _ButtonWidth, _BodyWidth;
    unsigned long _CurrentItem;
    unsigned int _CharacterSize;

    void centerText();
};

