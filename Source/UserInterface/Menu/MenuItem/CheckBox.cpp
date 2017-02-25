
#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/CheckBox.h"

Cross::Cross(sf::Vector2f size, sf::Color color) : _Visible(true) {
    sf::Vector2f barSize = sf::Vector2f(size.x * size.x + size.y * size.y, _BarHeight);
    _Left = sf::RectangleShape(barSize);
    _Left.setOrigin(barSize.x / 2, barSize.y / 2);
    _Left.setRotation(45);
    _Left.setFillColor(color);

    _Right = sf::RectangleShape(barSize);
    _Right.setOrigin(barSize.x / 2, barSize.y / 2);
    _Right.setRotation(-45);
    _Right.setFillColor(color);
}

void Cross::render(sf::RenderWindow &renderWindow) {
    if (_Visible) {
        renderWindow.draw(_Left);
        renderWindow.draw(_Right);
    }
}

CheckBox::CheckBox(sf::Vector2f pos, sf::Font &font, std::string text) : MenuItem(
        MenuItemType::MCheckbox, font) {

    _Text.setFont(font);
    _Text.setString(text);
    _Text.setOrigin(0, _Text.getLocalBounds().height / 2);

    _Cross = Cross(sf::Vector2f(5, 1), sf::Color::Black);

    _BoxBackground.setSize(sf::Vector2f(25, 25));
    _BoxBackground.setOrigin(_BoxBackground.getLocalBounds().width / 2, _BoxBackground.getLocalBounds().height / 2);
    _BoxBackground.setFillColor(sf::Color::Transparent);
    _BoxBackground.setOutlineColor(sf::Color::Black);
    _BoxBackground.setOutlineThickness(2.0f);

    float width = _BoxBackground.getLocalBounds().width + _BoxTextPadding + _Text.getLocalBounds().width;
    float boxOffset = width / 2 - _BoxBackground.getLocalBounds().width / 2;
    sf::Vector2f boxPosition(pos.x - boxOffset, pos.y);
    _BoxBackground.setPosition(boxPosition);
    _Cross.setPosition(boxPosition);
    _Text.setPosition(boxPosition.x + _BoxBackground.getLocalBounds().width / 2 + _BoxTextPadding, boxPosition.y - 5);
}

void CheckBox::render(sf::RenderWindow &renderWindow) {
    if (_ChangeCursor) {
        sf::StandardCursor cursor;
        cursor.set(renderWindow.getSystemHandle(), sf::StandardCursor::TYPE::HAND);
    }

    if (!_Enabled) {
        // TODO grey out the checkbox
    }

    renderWindow.draw(_Text);
    renderWindow.draw(_BoxBackground);
    _Cross.render(renderWindow);
}

bool CheckBox::handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {
    if (newEvent.type == sf::Event::MouseButtonPressed) {
        if (rh::pointInRectangle(_BoxBackground.getGlobalBounds(), mousePos)) {
            _Checked = !_Checked;
            _Cross.setVisible(!_Cross.isVisible());
            return true;
        }
    }
    MenuItem::updateCursor(newEvent, mousePos);
    return false;
}

sf::FloatRect CheckBox::getRect() {
    sf::FloatRect boundingBox;
    boundingBox.top = _BoxBackground.getGlobalBounds().top;
    boundingBox.left = _BoxBackground.getGlobalBounds().left;
    boundingBox.width = _BoxBackground.getLocalBounds().width + _Text.getLocalBounds().width;
    boundingBox.height = _BoxBackground.getLocalBounds().height;
    return boundingBox;
}

