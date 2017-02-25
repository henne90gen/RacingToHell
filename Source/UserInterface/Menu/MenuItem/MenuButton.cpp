#include <StandardCursor.h>
#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/MenuButton.h"

MenuButton::MenuButton(sf::Vector2f pos, sf::Vector2f size, sf::Font &font, std::string text,
                       TextAlignment align)
        : MenuItem(MenuItemType::MButton, font), _Alignment(align) {
    _Text.setFont(font);
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
}

void MenuButton::render(sf::RenderWindow &renderWindow) {
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

        if (_ChangeCursor) {
            sf::StandardCursor cursor;
            cursor.set(renderWindow.getSystemHandle(), sf::StandardCursor::TYPE::HAND);
        }

        if (_Background.getSize().x > 0) {
            renderWindow.draw(_Background);
        }

        renderWindow.draw(_Text);
    }
}

bool MenuButton::handleEvent(sf::Event &event, sf::Vector2f mousePos) {
    if (_Enabled && _Visible) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (rh::pointInRectangle(getRect(), mousePos)) {
                return true;
            }
        } else if (event.type == sf::Event::MouseMoved) {
            _Hovering = rh::pointInRectangle(getRect(), mousePos);
        }
    }
    MenuItem::updateCursor(event, mousePos);
    return false;
}

sf::FloatRect MenuButton::getRect() {
    return _Background.getGlobalBounds();
}

void MenuButton::setText(std::string text) {
    _Text.setString(text);
    _Text.setPosition(_Background.getPosition() +
                      sf::Vector2f(_Background.getLocalBounds().width / 2 - _Text.getLocalBounds().width / 2, 0));
}
