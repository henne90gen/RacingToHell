#pragma once

#include <future>

class Framework;

class LevelManager {
public:
    /**
     * Manages the level. Draws background and keeps track of the score
     */
    LevelManager(Framework &framework) : _FW(framework), _ShouldMove(true), _Score(0) {}

    ~LevelManager() {}

    /**
     * Updates the Level with the given frame time
     * @param frameTime Time that has passed since the last update
     */
    void update(float frameTime);

    void load();

    /**
     * Resets the Level to a state where a new game can be started
     */
    void resetToLevelOne();

    /**
     * Tells the Level to increase the difficulty and to change the background
     */
    void levelUp();

    /**
     * @return int Difficulty of the Level
     */
    int getLevel() { return _Level; }

    /**
     * @return int Speed of the road
     */
    int getRoadSpeed();

    /**
     * @return float Time in seconds that has passed since the start of the level
     */
    float getLevelTime() { return _LevelTime; }

    /**
     * @return float Time in seconds that this level will last in total
     */
    float getTotalLevelTime() { return _TotalLevelTime; }

    float getScore() { return _Score; }

    void addScore(ScoreEvent event, float modifier);

    int getAiHP();

    int getBossHP();

    sf::Sprite &getSprite() {
        return _Sprite;
    }

    /**
     * Controls the movement of the background.
     */
    void setMoving(bool moving) {
        _ShouldMove = moving;
    }

private:
    Framework &_FW;

    float _Score, _LevelTime, _TotalLevelTime;

    int _Level;

    bool _ShouldMove;

    sf::Texture *_Textures;

    sf::Sprite _Sprite;
};
