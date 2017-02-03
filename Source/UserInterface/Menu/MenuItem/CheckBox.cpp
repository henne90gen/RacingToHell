//
// Created by henne on 02.02.17.
//

#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/CheckBox.h"

CheckBox::CheckBox(sf::Vector2f pos, MenuResult action, sf::Font &font, std::string text) : MenuItem(
        MenuItemType::MCheckbox, action, font), _Cross(pos, sf::Vector2f(20, 5), sf::Color::Black) {
    _Text.setFont(_Font);
    _Text.setString(text);
    _Text.setPosition(pos);
}

void CheckBox::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Text);
    _Cross.render(renderWindow);
}

MenuResult CheckBox::handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {

    if (newEvent.type == sf::Event::MouseButtonPressed) {
//        if (pointInRectangle(_Sprite.getGlobalBounds(), mousePos)) {
//            _Checked = !_Checked;
//            return _Action;
//        }
    }
    return MenuResult::Nothing;
}

sf::FloatRect CheckBox::getRect() {
    sf::FloatRect boundingBox;
    return boundingBox;
}
