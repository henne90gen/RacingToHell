#pragma once

#include "MenuItem.h"

template<class T>
class ComboBox : public MenuItem<T> {
public:

    ComboBox() {}

    // TODO create more constructor options (width and not set, just height not set, etc.)
    ComboBox(sf::Vector2f pos, std::vector<std::string> itemStrings, std::vector<float> itemValues, sf::Font &font,
             int width, int height) : MenuItem<T>(MenuItemType::MCombobox, font),
                                      _ItemStrings(itemStrings), _ItemValues(itemValues), _Position(pos),
                                      _CurrentItem((T) 0), _Height(height), _BodyWidth(width - 40), _ButtonWidth(20),
                                      _CharacterSize(40), _BgColorEnabled(sf::Color(0, 0, 0, 175)),
                                      _BgColorHovering(sf::Color(50, 50, 50, 100)),
                                      _BgColorDisabled(sf::Color(0, 0, 0, 175)),
                                      _FgColorEnabled(sf::Color(255, 255, 255, 255)),
                                      _FgColorDisabled(sf::Color(150, 150, 150, 255)) {

        _Body.setPosition(pos + sf::Vector2f(_ButtonWidth, 0));
        _Body.setSize(sf::Vector2f(_BodyWidth, _Height));
        _Body.setOutlineThickness(1);
        _Body.setOutlineColor(sf::Color::Black);

        _ButtonLeft.setPosition(pos);
        _ButtonLeft.setSize(sf::Vector2f(_ButtonWidth, _Height));
        _ButtonLeft.setOutlineThickness(1);
        _ButtonLeft.setOutlineColor(sf::Color::Black);

        _ButtonRight.setPosition(pos + sf::Vector2f(_ButtonWidth + _BodyWidth, 0));
        _ButtonRight.setSize(sf::Vector2f(_ButtonWidth, _Height));
        _ButtonRight.setOutlineThickness(1);
        _ButtonRight.setOutlineColor(sf::Color::Black);

        if (_ArrowTexture.loadFromFile("Resources/Texture/MainMenu/ComboboxArrow.png")) {
            _LeftArrowSprite.setTexture(_ArrowTexture);
            _RightArrowSprite.setTexture(_ArrowTexture);

            _LeftArrowSprite.setTextureRect(
                    sf::IntRect(_ArrowTexture.getSize().x, 0, -1 * _ArrowTexture.getSize().x,
                                _ArrowTexture.getSize().y));

            _LeftArrowSprite.setPosition(pos + sf::Vector2f(0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x,
                                                            0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
            _RightArrowSprite.setPosition(pos + sf::Vector2f(
                    _BodyWidth + _ButtonWidth + 0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x,
                    0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
        }

        _Text.setFont(font);
        _Text.setCharacterSize(_CharacterSize);
        _Text.setString(_ItemStrings[(int) _CurrentItem]);
        centerText();
    }

    virtual ~ComboBox() {};

    void render(sf::RenderWindow &renderWindow) {
        if (this->_Enabled) {
            if (this->_HoveringLeft) {
                _ButtonLeft.setFillColor(_BgColorHovering);
                _ButtonRight.setFillColor(_BgColorEnabled);
            } else if (this->_HoveringRight) {
                _ButtonRight.setFillColor(_BgColorHovering);
                _ButtonLeft.setFillColor(_BgColorEnabled);
            } else {
                _ButtonRight.setFillColor(_BgColorEnabled);
                _ButtonLeft.setFillColor(_BgColorEnabled);
            }
            _Body.setFillColor(_BgColorEnabled);
            _Text.setFillColor(_FgColorEnabled);
        } else {
            _Body.setFillColor(_BgColorDisabled);
            _ButtonLeft.setFillColor(_BgColorDisabled);
            _ButtonRight.setFillColor(_BgColorDisabled);
            _Text.setFillColor(_FgColorDisabled);
        }

        renderWindow.draw(_ButtonLeft);
        renderWindow.draw(_ButtonRight);
        renderWindow.draw(_Body);
        renderWindow.draw(_Text);
        renderWindow.draw(_LeftArrowSprite);
        renderWindow.draw(_RightArrowSprite);
    }

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {
        if (this->_Enabled) {
            if (newEvent.type == sf::Event::MouseMoved) {
                _HoveringLeft = rh::pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos);
                _HoveringRight = rh::pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos);
                this->_Hovering = _HoveringLeft || _HoveringRight;
            }
            if (newEvent.type == sf::Event::MouseButtonPressed) {
                int currentItemIndex = (int) _CurrentItem;
                if (rh::pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos)) {
                    _CurrentItem = (T) ((currentItemIndex - 1 < 0) ? (long) _ItemStrings.size() - 1 : currentItemIndex -
                                                                                                      1);
                    _Text.setString(_ItemStrings[(int) _CurrentItem]);
                    centerText();
                    return true;
                } else if (rh::pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos)) {
                    _CurrentItem = (T) ((currentItemIndex + 1 >= _ItemStrings.size()) ? 0 : currentItemIndex + 1);
                    _Text.setString(_ItemStrings[(int) _CurrentItem]);
                    centerText();
                    return true;
                }
            }
        }
        return false;
    }

    sf::FloatRect getRect() {
        return _Body.getGlobalBounds();
    }

    /**
     * @return std::string Text of the Button
     */
    std::string getText() { return _ItemStrings[(int) _CurrentItem]; }

    /**
     * set currentItem
     * @return bool False
     */
    void setValue(T newValue) {
        _CurrentItem = newValue;
        _Text.setString(_ItemStrings[(int) _CurrentItem]);
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
    T getMaxValue() { return (T) 0; }

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

    void centerText() {
        _Text.setPosition(_Position +
                          sf::Vector2f(_ButtonWidth + 0.5f * _BodyWidth - 0.5f * _Text.getGlobalBounds().width, 0));
    }
};
