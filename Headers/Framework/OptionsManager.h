//
// Created by henne on 29.11.16.
//

#pragma once


#include <stdafx.h>

class OptionsManager {
public:

    OptionsManager();

    float getFPS() { return _FPS; }

    void setFPS(float fps) { _FPS = fps; }

    GameMode getGameMode() { return _GameMode; }

    void setGameMode(GameMode gm) { _GameMode = gm; }

    Difficulty getDifficulty() { return _Difficulty; }

    void setDifficulty(Difficulty newDiff) { _Difficulty = newDiff; }

    float getVolume() { return _Volume; }

    void setVolume(float volume) { _Volume = volume; }


    sf::Font &getFont() { return _Font; }

    void setFont(sf::Font font) { _Font = font; }

    bool isDebugOn() { return _Debug; }

    void loadOptions();

    void saveOptions();

    float getScoreMultiplier();

    std::vector<std::string> getDifficultyStrings() { return _DifficultyStrings; }

    std::vector<float> getDifficultyValues() { return _DifficultyValues; }

    std::vector<float> getGameModeValues() { return _GameModeValues; }

    std::vector<std::string> getGameModeStrings() { return _GameModeStrings; }

private:
    std::vector<float> _DifficultyValues = {0.5f, 1.0f, 1.5f, 2.0f};
    std::vector<std::string> _DifficultyStrings = {"Easy", "Normal", "Hard", "Insane"};

    std::vector<float> _GameModeValues = {1.0f, 0.0f, 0.5f, 3.0f};
    std::vector<std::string> _GameModeStrings = {"Standard", "Invincibility", "Infinite Energy", "Hardcore"};

    Difficulty _Difficulty;
    GameMode _GameMode;

    float _FPS;
    float _Volume;

    bool _Debug;

    std::string _MPName;
    sf::Font _Font;
};


