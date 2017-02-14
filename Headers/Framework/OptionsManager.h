#pragma once

#include "Manager.h"

class OptionsManager : public Manager {
public:

    OptionsManager(Framework &framework);

    float getFPS();

    void setFPS(float fps) { _FPS = fps; };

    bool isVSyncEnabled() { return _VSyncEnabled; }

    void setVSyncEnabled(bool vSync);

    float getVolume() { return _Volume; }

    void setVolume(float volume) { _Volume = volume; }

    GameMode getGameMode() { return _GameMode; }

    void setGameMode(GameMode gm) { _GameMode = gm; }

    Difficulty getDifficulty() { return _Difficulty; }

    void setDifficulty(Difficulty newDiff) { _Difficulty = newDiff; }

    sf::Font &getFont() { return _Font; }

    void setFont(sf::Font font) { _Font = font; }

    bool isDebugEnabled() { return _Debug; }

    void setDebugEnabled(bool debug) { _Debug = debug; }

    void load();

    void save();

    float getScoreMultiplier();

    /**
     * Names for the difficulty levels
     */
    std::vector<std::string> getDifficultyStrings() { return _DifficultyStrings; }

    /**
     * Values for the difficulty levels
     */
    std::vector<float> getDifficultyValues() { return _DifficultyValues; }

    /**
     * Names for all the GameModes
     */
    std::vector<std::string> getGameModeStrings() { return _GameModeStrings; }

    /**
     * Values for all the GameModes
     */
    std::vector<float> getGameModeValues() { return _GameModeValues; }

private:
    std::vector<float> _DifficultyValues = {0.5f, 1.0f, 1.5f, 2.0f};
    std::vector<std::string> _DifficultyStrings = {"Easy", "Normal", "Hard", "Insane"};

    std::vector<float> _GameModeValues = {1.0f, 0.0f, 0.5f, 3.0f};
    std::vector<std::string> _GameModeStrings = {"Standard", "Invincibility", "Infinite Energy", "Hardcore"};

    float _FPS = 60.0f;
    bool _VSyncEnabled = false;
    float _Volume = 30.0f;
    Difficulty _Difficulty = Difficulty::Normal;
    GameMode _GameMode = GameMode::Standard;
    bool _Debug = false;

    sf::Font _Font;
    const std::string _SettingsFileName = "Resources/settings.cfg";
};


