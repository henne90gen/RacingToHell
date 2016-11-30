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

    int getCurrentCarSkinIndex();

    void setCurrentCarSkinIndex(int newIndex);

    float getVolume();

    void setVolume(float volume);

    std::vector<float> getScoreMultiplierList();

    void loadOptions();

    void saveOptions();

private:
    std::vector<float> _ScoreMultiplierList;
    float _Score;
    GameMode _GameMode;
    int _Difficulty;
    int _CurrentCarSkinIndex;
    float _FPS;
    float _Volume;
    std::string _MPName;
};


