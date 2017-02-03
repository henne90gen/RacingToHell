//
// Created by henne on 22.01.17.
//

#include "stdafx.h"
#include "UserInterface/Screen/DebugScreen.h"
#include "Framework/Framework.h"

DebugScreen::DebugScreen(Framework &framework) : GameScreen(framework) {
    _LabelStrings = {"FPSIndex:",
                     "UPS:",
                     "CurrState:",
                     "LastState:",
                     "",
                     "Difficulty:",
                     "GameMode:",
                     "Score:",
                     "ScoreMulti:",
                     "",
                     "Bullets:",
                     "Cars:",
                     "AI health:",
                     "RoadSpeed:",
                     "",
                     "Frequencies",
                     "Car freq:",
                     "Bullet freq:",
                     "Can. freq:",
                     "Tool. freq:"};

    sf::Font &font = _FW.getOptionsManager().getFont();

    float descriptorWidth = 0;
    for (unsigned long i = 0; i < _LabelStrings.size(); i++) {
        sf::Text label = sf::Text(_LabelStrings.at(i), font, _CharacterSize);
        label.setPosition(_XOffset, _YOffset + i * _VerticalSpacing);
        if (label.getLocalBounds().width > descriptorWidth) {
            descriptorWidth = label.getLocalBounds().width;
        }
        _Labels.push_back(label);
    }

    for (unsigned int i = 0; i < _LabelStrings.size(); i++) {
        sf::Text value = sf::Text("", font, _CharacterSize);
        value.setPosition(_XOffset + _HorizontalSpacing + descriptorWidth, _YOffset + i * _VerticalSpacing);
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

    std::vector<long> skipping;
    std::vector<std::string> newValues;

    newValues.push_back(std::to_string(_FW.getFPS())); // FPSIndex
    newValues.push_back(std::to_string(_FW.getUPS())); // UPS

    newValues.push_back(std::to_string((int) _FW.getCurrentGameState())); // Current GameState
    newValues.push_back(std::to_string((int) _FW.getLastGameState())); // Last GameState

    newValues.push_back("");  // Empty line

    newValues.push_back(std::to_string((int) _FW.getOptionsManager().getDifficulty())); // Difficulty
    newValues.push_back(std::to_string((int) _FW.getOptionsManager().getGameMode())); // GameMode

    newValues.push_back(std::to_string(_FW.getLevelManager().getScoref())); // Score
    newValues.push_back(std::to_string(_FW.getOptionsManager().getScoreMultiplier())); // ScoreMultiplier

    newValues.push_back(""); // Empty line

    unsigned long bullets = _FW.getGOM().getBullets().size();
    newValues.push_back(std::to_string(bullets)); // Bullet amount
    unsigned long cars = _FW.getGOM().getCars().size() + 1;
    newValues.push_back(std::to_string(cars)); // Car amount
    newValues.push_back(std::to_string(_FW.getLevelManager().getAiHP())); // Enemy health
    newValues.push_back(std::to_string(_FW.getLevelManager().getRoadSpeed())); // RoadSpeed

    newValues.push_back(""); // Empty line
    newValues.push_back(""); // Frequencies heading

    newValues.push_back(std::to_string(_FW.getGOM().getCarFrequency())); // Car frequency
    newValues.push_back(std::to_string(_FW.getGOM().getBulletFrequency())); // Bullet frequency
    newValues.push_back(std::to_string(_FW.getGOM().getCanisterFrequency())); // Canister frequency
    newValues.push_back(std::to_string(_FW.getGOM().getToolboxFrequency())); // Toolbox frequency

    for (unsigned int i = 0; i < _Values.size(); i++) {
        if (i >= newValues.size()) { break; }
        _Values.at(i).setString(newValues.at(i));
    }
}
