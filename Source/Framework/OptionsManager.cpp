//
// Created by henne on 29.11.16.
//

#include "stdafx.h"
#include "Framework/OptionsManager.h"

OptionsManager::OptionsManager() {
    _Font.loadFromFile("Resources/Font/arial.ttf");

    // Standard configuration if there is no settings.cfg file
    _Volume = 20;
    _Difficulty = Difficulty::Normal;
    _GameMode = GameMode::Standard;
    _Debug = true;
}

inline bool file_exists(const std::string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

void OptionsManager::loadOptions() {

    std::vector<std::string> settings;
    std::string option;
    std::ifstream fileStream;

    if (!file_exists(_SettingsFileName)) {
        saveOptions();
        return;
    }

    fileStream.open(_SettingsFileName);
    while (std::getline(fileStream, option)) {
        settings.push_back(option);
    }
    fileStream.close();

    if (settings.size() >= 5) {
        _Volume = std::stof(settings[0]);
        _Difficulty = (Difficulty) std::stoi(settings[1]);
        _GameMode = (GameMode) std::stoi(settings[2]);
        _Debug = (bool) std::stoi(settings[3]);
    }
    // FIXME Check settings integrity (Diff or Mode could be out of range)
}

void OptionsManager::saveOptions() {
    std::ofstream fileStream;
    fileStream.open(_SettingsFileName);

    fileStream << _Volume << std::endl;
    fileStream << (int) _Difficulty << std::endl;
    fileStream << (int) _GameMode << std::endl;
    fileStream << _Debug << std::endl;

    fileStream.close();
}

float OptionsManager::getScoreMultiplier() {
    return _DifficultyValues[(int) _Difficulty] * _GameModeValues[(int) _GameMode];
}
