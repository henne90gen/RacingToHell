//
// Created by henne on 29.11.16.
//

#include "Framework/OptionsManager.h"

OptionsManager::OptionsManager() {
    // FIXME read startup config from file
    _FPS = 60;
    _Volume = 20;
    _GameMode = GameMode::Standard;

    _Font.loadFromFile("Resources/Font/arial.ttf");
}

void OptionsManager::loadOptions() {

    std::vector<std::string> settings;
    std::string option;
    std::ifstream fileStream;

    fileStream.open("Resources/Data/settings.cfg");
    while (std::getline(fileStream, option))
    {
        settings.push_back(option);
    }
    fileStream.close();

    if (settings.size() >= 3)
    {
        _FPS = std::stoi(settings[0]);
        _Volume = std::stof(settings[1]);
        _Difficulty = std::stoi(settings[2]);

        if (settings.size() >= 4)
        {
            _MPName = settings[3];
        }
    }
}

void OptionsManager::saveOptions() {
    std::string Path = "Resources/Data/Settings.cfg";
    std::ofstream FileStream;
    FileStream.open(Path);

    FileStream << _FPS << std::endl << _Volume << std::endl << _Difficulty << std::endl/* << _MPName */;

    FileStream.close();
}

void OptionsManager::setDifficulty(int newDiff) {
    _Difficulty = newDiff;
}

float OptionsManager::getFPS() {
    return _FPS;
}

void OptionsManager::setFPS(float fps) {
    _FPS = fps;
}

float OptionsManager::getScore() {
    return _Score;
}

void OptionsManager::setScore(float score) {
    _Score = score;
}

GameMode OptionsManager::getGameMode() {
    return _GameMode;
}

void OptionsManager::setGameMode(GameMode gm) {
    _GameMode = gm;
}

int OptionsManager::getDifficulty() {
    return _Difficulty;
}

float OptionsManager::getVolume() {
    return _Volume;
}

void OptionsManager::setVolume(float volume) {
    _Volume = volume;
}

std::vector<float> OptionsManager::getScoreMultiplierList() { return _ScoreMultiplierList; }

sf::Font &OptionsManager::getFont() {
    return _Font;
}

void OptionsManager::setFont(sf::Font font) {
    _Font = font;
}
