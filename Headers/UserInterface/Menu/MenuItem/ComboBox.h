#pragma once

#include "MenuItem.h"

template<class T>
class ComboBox : public MenuItem<T> {
public:
    ComboBox(sf::Vector2f pos, std::vector <std::string> itemStrings, std::vector<float> itemValues, sf::Font &font,
             int width, int height) {}

    virtual ~ComboBox() {};

    void render(sf::RenderWindow &renderWindow) {}

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {}

    sf::FloatRect getRect() {}

    /**
     * @return std::string Text of the Button
     */
    std::string getText() { return _ItemStrings[_CurrentItem]; }

    /**
     * set currentItem
     * @return bool False
     */
    void setValue(T newValue) {
        _CurrentItem = newValue;
        _Text.setString(_ItemStrings[_CurrentItem]);
        centerText();
    }

    /**
     * return current Item
     * @return float 0.0f
     */
    T getValue() { return _CurrentItem; }

    /**
     * Combo doesn't have a maximum value that could be returned
     * @return float 0.0f
     */
    T getMaxValue() { return nullptr; }

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

    T _CurrentItem;
    unsigned int _CharacterSize;

    void centerText() {}
};

