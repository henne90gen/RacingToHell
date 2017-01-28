//
// Created by henne on 22.01.17.
//

#include "stdafx.h"
#include "UserInterface/Screen/DebugScreen.h"
#include "Framework/Framework.h"

DebugScreen::DebugScreen(Framework &framework) : GameScreen(framework) {
    sf::Font &font = _FW.getOptionsManager().getFont();

    for (unsigned long i = 0; i < _LabelStrings.size(); i++) {
        sf::Text label = sf::Text(_LabelStrings.at(i), font, _CharacterSize);
        label.setPosition(_XOffset, _YOffset + i * _VerticalSpacing);
        _Labels.push_back(label);
    }

    for (unsigned int i = 0; i < _LabelStrings.size(); i++) {
        sf::Text value = sf::Text("", font, _CharacterSize);
        value.setPosition(_XOffset + _HorizontalSpacing, _YOffset + i * _VerticalSpacing);
        _Values.push_back(value);
    }
}

void DebugScreen::render(sf::RenderWindow &renderWindow) {
    for (unsigned int i = 0; i < _Labels.size(); i++) {
        renderWindow.draw(_Labels.at(i));
    }
    for (unsigned int i = 0; i < _Values.size(); i++) {
        renderWindow.draw(_Values.at(i));
    }
}

void DebugScreen::handleEvent(sf::Event &event) {

}

void DebugScreen::update(float frameTime) {
    std::vector<std::string> newValues;
    newValues.push_back(std::to_string(_FW.getFPS()));
    newValues.push_back(std::to_string(_FW.getUPS()));

    newValues.push_back(std::to_string((int) _FW.getCurrentGameState()));
    newValues.push_back(std::to_string((int) _FW.getLastGameState()));

    unsigned long bullets = _FW.getGOM().getBullets().size();
    newValues.push_back(std::to_string(bullets));

    unsigned long cars = _FW.getGOM().getCars().size() + 1;
    newValues.push_back(std::to_string(cars));

    newValues.push_back(std::to_string(_FW.getLevelManager().getAiHP()));
    newValues.push_back(std::to_string(_FW.getGOM().getCarFrequency()));
    newValues.push_back(std::to_string(_FW.getGOM().getBulletFrequency()));
    newValues.push_back(std::to_string(_FW.getGOM().getCanisterFrequency()));
    newValues.push_back(std::to_string(_FW.getGOM().getToolboxFrequency()));


    for (unsigned int i = 0; i < _Values.size(); i++) {
        if (i >= newValues.size()) {
            break;
        }
        _Values.at(i).setString(newValues.at(i));
    }
}
