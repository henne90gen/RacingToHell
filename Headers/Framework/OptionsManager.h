//
// Created by henne on 29.11.16.
//

#pragma once


#include <stdafx.h>

class OptionsManager {
public:

    OptionsManager();

    float getFPS();

    void setFPS(float fps);

    float getScore();

    void setScore(float score);

    GameMode getGameMode();

    void setGameMode(GameMode gm);

    int getDifficulty();

    void setDifficulty(int newDiff);

    float getVolume();

    void setVolume(float volume);

    std::vector<float> getScoreMultiplierList();

    sf::Font &getFont();

    void setFont(sf::Font font);

    bool isDebugOn();

    void loadOptions();

    void saveOptions();

private:
    std::vector<float> _ScoreMultiplierList;
    float _Score;
    GameMode _GameMode;
    int _Difficulty;
    float _FPS;
    float _Volume;
    bool _Debug;
    std::string _MPName;
    sf::Font _Font;
};


