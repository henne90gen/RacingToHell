#pragma once

#include "MenuItem.h"
#include "MenuButton.h"

template<class T>
class ComboBox : public MenuItem<T> {
public:

    ComboBox() {}

    // TODO create more constructor options (width and not set, just height not set, etc.)
    ComboBox(sf::Vector2f pos, std::vector<std::string> itemStrings, std::vector<float> itemValues, sf::Font &font,
             int width, int height) : MenuItem<T>(MenuItemType::MCombobox, font),
                                      _ItemStrings(itemStrings),
                                      _ItemValues(itemValues),
                                      _Position(pos),
                                      _CurrentItem((T) 0),
                                      _Height(height),
                                      _ButtonWidth(30),
                                      _BodyWidth(width - _ButtonWidth * 2),
                                      _CharacterSize(40),
                                      _BgColorEnabled(sf::Color(0, 0, 0, 175)),
                                      _BgColorDisabled(sf::Color(0, 0, 0, 175)),
                                      _FgColorEnabled(sf::Color(255, 255, 255, 255)),
                                      _FgColorDisabled(sf::Color(150, 150, 150, 255)) {

        _Body.setPosition(pos + sf::Vector2f(_ButtonWidth, 0));
        _Body.setSize(sf::Vector2f(_BodyWidth, _Height));
        _Body.setOutlineThickness(1);
        _Body.setOutlineColor(sf::Color::Black);

        sf::Vector2f buttonSize = sf::Vector2f(_ButtonWidth, _Height);
        _LeftBtn = MenuButton(pos + sf::Vector2f(_ButtonWidth / 2, 0), buttonSize, font, "<", TextAlignment::Center);
        _RightBtn = MenuButton(pos + sf::Vector2f(_ButtonWidth + _BodyWidth + _ButtonWidth / 2, 0), buttonSize, font,
                               ">", TextAlignment::Center);

        _Text.setFont(font);
        _Text.setCharacterSize(_CharacterSize);
        _Text.setString(_ItemStrings[(int) _CurrentItem]);
        centerText();
    }

    virtual ~ComboBox() {};

    void render(sf::RenderWindow &renderWindow) {
        // 'this->' is necessary, explanation: http://stackoverflow.com/questions/7076169/not-declared-in-this-scope-error-with-templates-and-inheritance
        if (this->_Enabled) {
            _Body.setFillColor(_BgColorEnabled);
            _Text.setFillColor(_FgColorEnabled);
        } else {
            _Body.setFillColor(_BgColorDisabled);
            _Text.setFillColor(_FgColorDisabled);
        }

        renderWindow.draw(_Body);
        renderWindow.draw(_Text);

        _LeftBtn.render(renderWindow);
        _RightBtn.render(renderWindow);
    }

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {
        if (this->_Enabled) {
            int currentItemIndex = (int) _CurrentItem;
            if (_LeftBtn.handleEvent(newEvent, mousePos)) {
                _CurrentItem = (T) ((currentItemIndex - 1 < 0) ? (long) _ItemStrings.size() - 1 : currentItemIndex - 1);
                _Text.setString(_ItemStrings[(int) _CurrentItem]);
                centerText();
                return true;
            }
            if (_RightBtn.handleEvent(newEvent, mousePos)) {
                _CurrentItem = (T) ((currentItemIndex + 1 >= _ItemStrings.size()) ? 0 : currentItemIndex + 1);
                _Text.setString(_ItemStrings[(int) _CurrentItem]);
                centerText();
                return true;
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

    MenuButton _LeftBtn, _RightBtn;
    sf::RectangleShape _Body;
    sf::Vector2f _Position;
    sf::Text _Text;
    sf::Color _BgColorEnabled, _BgColorDisabled;
    sf::Color _FgColorEnabled, _FgColorDisabled;

    int _Height, _ButtonWidth, _BodyWidth;

    T _CurrentItem;
    unsigned int _CharacterSize;

    void centerText() {
        _Text.setPosition(
                _Position + sf::Vector2f(_ButtonWidth + 0.5f * _BodyWidth - 0.5f * _Text.getGlobalBounds().width, 0));
    }
};
