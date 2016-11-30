//
// Created by henne on 29.11.16.
//

#include "Framework/OptionsManager.h"

OptionsManager::OptionsManager() {
    // FIXME read startup config from file
    _FPS = 60;
    _Volume = 20;
    _GameMode = GameMode::Standard;
    _CurrentCarSkinIndex = 0;
}

void OptionsManager::loadOptions() {
    std::vector<std::string> Settings;
    std::string Option;
    std::ifstream FileStream;

    FileStream.open("Resources/Data/Settings.cfg");
    while (std::getline(FileStream, Option))
    {
        Settings.push_back(Option);
    }
    FileStream.close();

    if (Settings.size() >= 3)
    {
        _FPS = std::stoi(Settings[0]);
        _Volume = std::stof(Settings[1]);
        _Difficulty = std::stoi(Settings[2]);

        if (Settings.size() >= 4)
        {
            _MPName = Settings[3];
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

int OptionsManager::getCurrentCarSkinIndex() {
    return _CurrentCarSkinIndex;
}

void OptionsManager::setCurrentCarSkinIndex(int newIndex) {
    _CurrentCarSkinIndex = newIndex;
}

float OptionsManager::getVolume() {
    return _Volume;
}

void OptionsManager::setVolume(float volume) {
    _Volume = volume;
}

std::vector<float> OptionsManager::getScoreMultiplierList() { return _ScoreMultiplierList; }
