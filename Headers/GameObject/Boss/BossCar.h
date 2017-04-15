#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/GameObjectType.h"
#include "GameObject/Bullet.h"
#include "Animation/Explosion.h"
#include "GameObject/Boss/Action/BossAction.h"

class BossCar : public Car {
public:
    /**
     * BossCar is the interface for every boss. It handles creation and sprite updating.
     * @param pos				Starting position
     * @param difficulty		selected difficulty
     * @param health			Starting health of the boss
     * @param texture			Texture for the sprite that is going to be used for collision detection
     * @param bulletTexture	Texture for the bullets the boss will shoot
     */
    BossCar(unsigned int id, GameObjectManager &gom, sf::Vector2f pos, int difficulty, int health, float speed,
            sf::Texture &texture,
            sf::Texture &bulletTexture);

//    BossCar(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, PlayerCar &player);

    virtual ~BossCar() {}

    /**
     * Renders the boss to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &window) = 0;

    /**
     * Updates the boss with the given frame time
     * @param frameTime Time that has passed since the last update
     * @param roadSpeed Velocity of the road
     * @param gameObjects Reference to the vector with all the game objects, so that the boss can add the bullets it shoots
     */
    virtual void update(float frameTime);

//    virtual void update(float frameTime);

    /**
     * Returns true if the boss allows traffic
     */
    bool hasTraffic() { return _HasTraffic; }

    /**
     * Returns true if the boss is dead and all explosion animations are done playing
     */
    bool isDoneExploding();

    sf::Vector2f getDefaultPosition() { return _DefaultPosition; }

    void setDefaultPosition(sf::Vector2f pos) { _DefaultPosition = pos; }

    /**
     * Writes the necessary data for a gameobject to a packet
     */
    void operator>>(sf::Packet &packet);

    /**
     * Reads the necessary data for a gameobject from a packet
     */
    void operator<<(sf::Packet &packet);

    virtual void init() = 0;

    void initBoss();

    virtual void updateActions() = 0;

    void shootBullet(sf::Vector2f pos, sf::Vector2f dir);

    sf::Vector2f getGunEnd();

    sf::Vector2f getGunDirection();

    sf::Vector2f getGunBasePosition() { return getPosition() + _GunOffset; }

    void rotateGunTowards(sf::Vector2f newDirection);

    /**
     * -1 means clockwise rotation and 1 means counterclockwise
     */
    void setGunMovement(int movement);

    int getGunMovement() { return _GunMovement; }

    void addAction(std::shared_ptr<BossAction> action) { _Actions.push_back(action); }

    GameObjectManager &getGOM() { return _GOM; }

protected:
//    enum Phase {
//        NOTHING,
//        SIMPLESHOOT,
//        SALVE,
//        SPIN,
//        HARDCORESPAM,
//        BLASTSALVE,
//        SPIRAL,
//        RANDOMSPRAY,
//        SHOTGUN,
//        SIDE,
//        SAVELANES,
//        RUNATPLAYERPHASE,
//        ZICKZACKPHASE,
//        SALVEZICKZACK
//    };

//    int _CurrentPhase;
//    std::vector<std::pair<Phase, float>> _Pattern;
//    sf::Clock _PhaseClock;

    sf::Uint16 _Difficulty;

//    enum Movement {
//        STILL, DRIVETODEFAULT, LEFTRIGHT, SWITCHSIDES, STRAIGHT, PARABOLA, RUNATPLAYER, ZICKZACK
//    };
//    Movement _MovementCommand;

    std::vector<std::shared_ptr<BossAction>> _Actions;
    unsigned int _NextPhase;

    // Points to the base position of the gun
    sf::Vector2f _GunOffset;

    // Points from the gun position to the end of the gun (where the bullets spawn)
    sf::Vector2f _GunDirection;
    float _GunLength;

    int _GunMovement;

    // FIXME initialize this in the constructor
    float _GunSpeed = 0.8f;

    sf::Vector2f _DefaultPosition;

    sf::RectangleShape _HealthBar;
    sf::RectangleShape _HealthBarFrame;

    sf::Texture _BulletTexture;

    int const _BulletSpeed;

    std::vector<std::shared_ptr<Explosion>> _Explosions;
    sf::Clock _ExplosionTimer;
    int _IsExploding;

//    sf::Clock _BossEventTimer1;
//    sf::Clock _BossEventTimer2;
//    float _Event1Frequency, _Event2Frequency;
//    bool _Event1Switch, _Event2Switch;
//    int _Event1Counter, _Event2Counter;

    sf::Vector2f _NextPosition;
    bool _Attack, _HasTraffic;

    float playerAngle(GameObject &Player);

    bool driveToNextPosition(float FrameTime);

    int getBossEvent();

    void updateHealthBar();

    virtual void checkPhase();

    void updateExplosions(float FrameTime);
};
