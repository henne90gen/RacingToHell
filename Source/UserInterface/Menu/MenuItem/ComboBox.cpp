#include "stdafx.h"

#include "UserInterface/Menu/MenuItem/ComboBox.h"

ComboBox::ComboBox(sf::Vector2f pos, std::vector<std::string> itemStrings, std::vector<float> itemValues,
                   MenuResult action, sf::Font &font, int width, int height)
        : MenuItem(MenuItemType::MCombobox, action, font), _ItemStrings(itemStrings), _ItemValues(itemValues),
          _Position(pos),
          _CurrentItem(0), _Height(height), _BodyWidth(width - 40), _ButtonWidth(20), _CharacterSize(40),
          _BgColorEnabled(sf::Color(0, 0, 0, 175)), _BgColorHovering(sf::Color(50, 50, 50, 100)),
          _BgColorDisabled(sf::Color(0, 0, 0, 175)),
          _FgColorEnabled(sf::Color(255, 255, 255, 255)), _FgColorDisabled(sf::Color(150, 150, 150, 255)) {
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
                sf::IntRect(_ArrowTexture.getSize().x, 0, -1 * _ArrowTexture.getSize().x, _ArrowTexture.getSize().y));

        _LeftArrowSprite.setPosition(pos + sf::Vector2f(0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x,
                                                        0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
        _RightArrowSprite.setPosition(pos + sf::Vector2f(
                _BodyWidth + _ButtonWidth + 0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x,
                0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
    }

    _Text.setFont(_Font);
    _Text.setCharacterSize(_CharacterSize);
    _Text.setString(_ItemStrings[_CurrentItem]);
    centerText();
}

void ComboBox::render(sf::RenderWindow &renderWindow) {
    if (_Enabled) {
        if (_HoveringLeft) {
            _ButtonLeft.setFillColor(_BgColorHovering);
            _ButtonRight.setFillColor(_BgColorEnabled);
        } else if (_HoveringRight) {
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

MenuResult ComboBox::handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {
    if (_Enabled) {
        if (newEvent.type == sf::Event::MouseMoved) {
            _HoveringLeft = pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos);
            _HoveringRight = pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos);
            _Hovering = _HoveringLeft || _HoveringRight;
        }
        if (newEvent.type == sf::Event::MouseButtonPressed) {
            if (pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos)) {
                _CurrentItem = (_CurrentItem - 1 < 0) ? (long) _ItemStrings.size() - 1 : _CurrentItem - 1;
                _Text.setString(_ItemStrings[_CurrentItem]);
                centerText();
                return _Action;
            } else if (pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos)) {
                _CurrentItem = (_CurrentItem + 1 >= _ItemStrings.size()) ? 0 : _CurrentItem + 1;
                _Text.setString(_ItemStrings[_CurrentItem]);
                centerText();
                return _Action;
            }
        }
    }
    return MenuResult::Nothing;
}

sf::FloatRect ComboBox::getRect() {
    return _Body.getGlobalBounds();
}

void ComboBox::centerText() {
    _Text.setPosition(_Position +
                      sf::Vector2f(_ButtonWidth + 0.5f * _BodyWidth - 0.5f * _Text.getGlobalBounds().width, 0));
}

void ComboBox::setValue(float x) {
    _CurrentItem = ((int) x % _ItemStrings.size());
    _Text.setString(_ItemStrings[_CurrentItem]);
    centerText();
}
