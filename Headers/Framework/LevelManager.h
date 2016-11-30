#pragma once

#include <future>

class LevelManager {
public:
    /*
        Manager class for the level, draws background and plays music
    */
    LevelManager() {}

    ~LevelManager() {}

    /*
        Updates the Level with the given frame time
        @param frameTime Time that has passed since the last update
        @param gameState State of the game
        @return bool True if 'level up' has happened
    */
    bool update(float frameTime, GameState gameState);

    /*
        Plays the level background music
    */
    void playMusic();

    /*
        Stops the Level background music
    */
    void stopMusic();

    /*
        Pauses the Level background music
    */
    void pauseMusic();

    /*
        Sets the volume to the given value
        @param volume New Volume value
    */
    void setVolume(float volume);

    /*
        Resets the Level to a state where a new game can be started
    */
    void resetToLevelOne();

    bool isResettingLevel() {
        return _IsResettingLevel;
    }

    void stopResettingLevel() {
        _IsResettingLevel = false;
    }

    /*
        Tells the Level to increase the difficulty and to change the background
    */
    void levelUp();

    /*
        @return int Difficulty of the Level
    */
    int getLevel() { return _Level; }

    /*
        @return int Speed of the road
    */
    int getRoadSpeed();

    /*
        Resets the internal timer of the Level
    */
    void resetTimer() { _LevelTime = 0; }

    /*
        Sets the difficulty of the level to the selected difficulty
        @param dif Difficulty
    */
    void setDifficulty(int dif) { _Difficulty = dif; }

    /*
        @return float Time in seconds that has passed since the start of the level
    */
    float getLevelTime() { return _LevelTime; }

    /*
        @return float Time in seconds that this level will last in total
    */
    float getTotalLevelTime() { return _TotalLevelTime; }

    sf::Vector2f getSpritePosition() {
        return _SpritePosition;
    }

private:
    sf::Music _Music;

    float _LevelTime;
    float _TotalLevelTime;

    int _Level, _Difficulty;

    std::mutex _ThreadGuard;
    bool _IsResettingLevel;

    sf::Vector2f _SpritePosition;
};
