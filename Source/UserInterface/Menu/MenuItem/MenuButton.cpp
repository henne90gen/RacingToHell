#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/MenuButton.h"

MenuButton::MenuButton(sf::Vector2f pos, sf::Vector2f size, MenuResult action, sf::Font &font, std::string text,
                       TextAlignment align)
        : MenuItem(MenuItemType::MButton, action, font), _Alignment(align) {
    _Text.setFont(_Font);
    _Text.setPosition(pos);
    _Text.setCharacterSize((unsigned int) (0.8f * size.y));
    _Text.setString(text);

    _Background.setPosition(pos);
    _Background.setSize(size);
    _Background.setOutlineThickness(1);
    _Background.setOutlineColor(sf::Color::Black);

    switch (_Alignment) {
        case TextAlignment::Center:
            _Background.setPosition(
                    _Background.getPosition() - sf::Vector2f(_Background.getLocalBounds().width / 2, 0));
            break;
        case TextAlignment::Right:
            _Background.setPosition(_Background.getPosition() - sf::Vector2f(_Background.getLocalBounds().width, 0));
            break;
        default:
            break;
    }

    _Text.setPosition(_Background.getPosition() +
                      sf::Vector2f(_Background.getLocalBounds().width / 2 - _Text.getLocalBounds().width / 2, 0));

    _Visible = true;
}

void MenuButton::render(sf::RenderWindow &window) {
    if (_Visible) {
        if ((_Hovering && _Enabled) || (_Focused && _Enabled)) {
            _Background.setFillColor(sf::Color(50, 50, 50, 100));
            _Text.setFillColor(sf::Color::White);
        } else if (_Enabled) {
            _Background.setFillColor(sf::Color(0, 0, 0, 175));
            _Text.setFillColor(sf::Color::White);
        } else {
            _Background.setFillColor(sf::Color(0, 0, 0, 100));
            _Text.setFillColor(sf::Color(150, 150, 150, 255));
        }

        if (_Background.getSize().x > 0) {
            window.draw(_Background);
        }

        window.draw(_Text);
    }

}

MenuResult MenuButton::handleEvent(sf::Event &event, sf::Vector2f mousePos) {
    if (_Enabled && _Visible) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (mousePos.y > getRect().top && mousePos.y < getRect().top + getRect().height &&
                mousePos.x > getRect().left && mousePos.x < getRect().left + getRect().width) {
                return _Action;
            }
        } else if (event.type == sf::Event::JoystickButtonPressed) {
            if (event.joystickButton.button == 0 && _Focused) {
                return _Action;
            }
        } else if (event.type == sf::Event::MouseMoved) {
            _Hovering = mousePos.y > getRect().top && mousePos.y < getRect().top + getRect().height &&
                        mousePos.x > getRect().left && mousePos.x < getRect().left + getRect().width;

            _Focused = false; //?
        }
    }
    return MenuResult::Nothing;
}

sf::FloatRect MenuButton::getRect() {
    if (_Action == MenuResult::NextSkin || _Action == MenuResult::PreviousSkin) {
        return _Text.getGlobalBounds();
    } else {
        return _Background.getGlobalBounds();
    }
}

void MenuButton::setText(std::string text) {
    _Text.setString(text);
    _Text.setPosition(_Background.getPosition() +
                      sf::Vector2f(_Background.getLocalBounds().width / 2 - _Text.getLocalBounds().width / 2, 0));
}
