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

    _Debug = true;
}

void OptionsManager::loadOptions() {

    std::vector<std::string> settings;
    std::string option;
    std::ifstream fileStream;

    fileStream.open("Resources/settings.cfg");
    while (std::getline(fileStream, option)) {
        settings.push_back(option);
    }
    fileStream.close();

    if (settings.size() >= 3) {
        _FPS = std::stoi(settings[0]);
        _Volume = std::stof(settings[1]);
        _Difficulty = (Difficulty) std::stoi(settings[2]);
        _GameMode = (GameMode) std::stoi(settings[3]);
        if (settings.size() >= 4) {
            _MPName = settings[3];
        }
    }
}

void OptionsManager::saveOptions() {
    std::string Path = "Resources/settings.cfg";
    std::ofstream fileStream;
    fileStream.open(Path);

    fileStream << _FPS << std::endl;
    fileStream << _Volume << std::endl;
    fileStream << (int) _Difficulty << std::endl;
    fileStream << (int) _GameMode << std::endl;
//    fileStream << _MPName << std::endl;

    fileStream.close();
}

float OptionsManager::getScoreMultiplier() {
    return _DifficultyValues[(int) _Difficulty] * _GameModeValues[(int) _GameMode];
}
