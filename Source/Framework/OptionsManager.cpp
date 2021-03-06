//
// Created by henne on 29.11.16.
//

#include "stdafx.h"
#include "Framework/OptionsManager.h"
#include "Framework/Framework.h"

OptionsManager::OptionsManager(Framework &framework) : Manager(framework) {
    if (!_Font.loadFromFile("Resources/Font/arial.ttf")) {
        std::cout << "Couldn't load font." << std::endl;
    }
}

void OptionsManager::load() {
    std::cout << "Loading settings..." << std::endl;

    std::vector<std::string> settings;
    std::string option;
    std::ifstream fileStream;

    if (!rh::file_exists(_SettingsFileName)) {
        save();
        setVSyncEnabled(_VSyncEnabled);
        return;
    }

    fileStream.open(_SettingsFileName);
    while (std::getline(fileStream, option)) {
        settings.push_back(option);
    }
    fileStream.close();

    if (settings.size() >= 6) {
        _FPS = std::stoi(settings[0]);
        _VSyncEnabled = (bool) std::stoi(settings[1]);
        _Volume = std::stof(settings[2]);
        _Difficulty = (Difficulty) std::stoi(settings[3]);
        _GameMode = (GameMode) std::stoi(settings[4]);
        _Debug = (bool) std::stoi(settings[5]);
    }
    // FIXME Check settings integrity (Diff or Mode could be out of range)

    setVSyncEnabled(_VSyncEnabled);
}

void OptionsManager::save() {
    std::ofstream fileStream;
    fileStream.open(_SettingsFileName);

    fileStream << _FPS << std::endl;
    fileStream << _VSyncEnabled << std::endl;
    fileStream << _Volume << std::endl;
    fileStream << (int) _Difficulty << std::endl;
    fileStream << (int) _GameMode << std::endl;
    fileStream << _Debug << std::endl;

    fileStream.close();
}

float OptionsManager::getScoreMultiplier() {
    return _DifficultyValues[(int) _Difficulty] * _GameModeValues[(int) _GameMode];
}

float OptionsManager::getFPS() {
    if (_VSyncEnabled) {
        return 60.0f;
    }
    return _FPS;
}

void OptionsManager::setVSyncEnabled(bool vSync) {
    _VSyncEnabled = vSync;
    _FW.setVSyncEnabled(vSync);
}
