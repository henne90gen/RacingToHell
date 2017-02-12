//
// Created by henne on 22.01.17.
//

#include "stdafx.h"
#include "UserInterface/Screen/DebugScreen.h"
#include "Framework/Framework.h"

DebugScreen::DebugScreen(Framework &framework) : GameScreen(framework) {
    _LabelStrings = {"FPS:",
                     "CurrState:",
                     "LastState:",
                     "NumScreens:",
                     "",
                     "Options",
                     "FPSPreset:",
                     "VSync:",
                     "Volume:",
                     "Difficulty:",
                     "GameMode:",
                     "ScoreMulti:",
                     "Score:",
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
                     "Tool. freq:",
                     "",
                     "Sounds:",
                     "LevelMusic:"};

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
    std::vector<std::string> values;

    values.push_back(std::to_string(_FW.getFPS())); // FPS

    values.push_back(std::to_string((int) _FW.getCurrentGameState())); // Current GameState
    values.push_back(std::to_string((int) _FW.getLastGameState())); // Last GameState
    values.push_back(std::to_string(_FW.getNumScreens())); // NumScreens

    values.push_back("");  // Empty line
    values.push_back("");  // Options heading

    values.push_back(std::to_string(_FW.getOptionsManager().getFPS())); // FPSPreset
    values.push_back(std::to_string(_FW.getOptionsManager().isVSyncEnabled())); // VSync
    values.push_back(std::to_string(_FW.getOptionsManager().getVolume())); // Volume
    values.push_back(std::to_string((int) _FW.getOptionsManager().getDifficulty())); // Difficulty
    values.push_back(std::to_string((int) _FW.getOptionsManager().getGameMode())); // GameMode

    values.push_back(std::to_string(_FW.getOptionsManager().getScoreMultiplier())); // ScoreMultiplier
    values.push_back(std::to_string(_FW.getLevelManager().getScore())); // Score

    values.push_back(""); // Empty line

    unsigned long bullets = _FW.getGOM().getBullets().size();
    values.push_back(std::to_string(bullets)); // Bullet amount
    unsigned long cars = _FW.getGOM().getCars().size() + 1;
    values.push_back(std::to_string(cars)); // Car amount
    values.push_back(std::to_string(_FW.getLevelManager().getAiHP())); // Enemy health
    values.push_back(std::to_string(_FW.getLevelManager().getRoadSpeed())); // RoadSpeed

    values.push_back(""); // Empty line
    values.push_back(""); // Frequencies heading

    values.push_back(std::to_string(_FW.getGOM().getCarFrequency())); // Car frequency
    values.push_back(std::to_string(_FW.getGOM().getBulletFrequency())); // Bullet frequency
    values.push_back(std::to_string(_FW.getGOM().getCanisterFrequency())); // Canister frequency
    values.push_back(std::to_string(_FW.getGOM().getToolboxFrequency())); // Toolbox frequency

    values.push_back(""); // Empty line

    values.push_back(std::to_string(_FW.getSoundManager().getNumSoundEffects())); // Number of sound effects
    values.push_back(std::to_string(_FW.getSoundManager().getLevelMusicIndex())); // Level music

    for (unsigned int i = 0; i < _Values.size(); i++) {
        if (i >= values.size()) { break; }
        _Values.at(i).setString(values.at(i));
    }
}
