#pragma once

#include <future>

class Framework;

class LevelManager {
public:
    /*
        Manager class for the level, draws background and plays music
    */
    LevelManager(Framework &framework) : _FW(framework), _ShouldMove(true), _Score(0) {}

    ~LevelManager() {}

    /*
        Updates the Level with the given frame time
        @param frameTime Time that has passed since the last update
    */
    void update(float frameTime);

    void load();

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
        @return float Time in seconds that has passed since the start of the level
    */
    float getLevelTime() { return _LevelTime; }

    /*
        @return float Time in seconds that this level will last in total
    */
    float getTotalLevelTime() { return _TotalLevelTime; }

    int getScore() { return (int) _Score; }

    float getScoref() { return _Score; }

    void addScore(ScoreEvent event, float modifier);

    int getAiHP();

    int getBossHP();

    sf::Sprite &getSprite() {
        return _Sprite;
    }

    void stopMoving() {
        _ShouldMove = false;
    }

    void startMoving() {
        _ShouldMove = true;
    }

private:
    Framework &_FW;

    sf::Music _Music;

    float _Score;

    float _LevelTime;
    float _TotalLevelTime;

    int _Level;

    bool _IsResettingLevel;

    std::vector<std::shared_ptr<sf::Texture>> _Textures;
    sf::Sprite _Sprite;
    bool _ShouldMove;
};
