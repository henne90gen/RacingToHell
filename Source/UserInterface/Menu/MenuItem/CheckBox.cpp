
#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/CheckBox.h"

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
