#pragma once

#include "stdafx.h"
#include "Car.h"
#include "AICar.h"
#include "GameObject.h"
#include "GameObject/GameObjectType.h"
#include "PlayerStats.h"
#include "Animation/Explosion.h"

class PlayerCar : public Car {
public:
    /*
        Car that is controlled by the player
        @param selectedCar Index of the selected car to set the stats
        @param texture Texture that is going to be used for the sprite
    */
    PlayerCar(unsigned int id, GameObjectManager &gom, PlayerCarIndex selectedCar, sf::Texture &texture,
              bool infEnergy);

//    PlayerCar(sf::Packet &packet, std::vector<std::shared_ptr<sf::Texture>> &textures, sf::Texture &explosionTexture);

    ~PlayerCar() {}

    /*
        Renders the PlayerCar to the specified RenderWindow
        @param window Window to draw to
        @param renderCrosshair Determines whether various aiming utilities (crosshair and aim line) are going to be rendered
    */
    void render(sf::RenderWindow &window, bool renderCrosshair);

    /*
        Handles events for PlayerCar
        @param newEvent Event to be handled
    */
    void handleEvent(sf::Event &newEvent);

    /*
        Updates the PlayerCar with the given frame time
        @param frameTime Time that has passed since the last update
        @param roadSpeed Speed of the road
    */
    void update(float frameTime);

    /*
        Adds 20 to the players health if possible
    */
    void addHealth();

    /*
        @return float Energy of the player
    */
    float getEnergy() { return _Energy; }

    /*
        @return float Maximum amount of energy the player can have
    */
    int getMaxEnergy() { return _MaxEnergy; }

    /*
        Adds 50 to the players energy if possible
    */
    void addEnergy();

    /*
        @return int Damage of a bullet fired by the player
    */
    int getBulletdamage() { return _Bulletdamage; }

    /*
        Changes the players stats according to the id of the car he chose.
    */
    void setStats(PlayerCarIndex id);

    /*
        Resets the movemnt vector that is applied to the PlayerCar
    */
//    void resetMovement() { _Movement = sf::Vector2f(0, 0); }

    bool isAlive();

    /*
        Player loses Energy
    */
    void drainEnergy(float frameTime);

    /*
        Must be called after player shot
     */
    void drainShotEnergy();

    /*
        Fills up resources to its maximum
    */
    void resetResources() {
        _Health = _MaxHealth;
        _Energy = _MaxEnergy;
    }

    /*
        Writes the necessary data for the playercar to a packet
    */
    void operator>>(sf::Packet &packet);

    /*
        Reads the necessary data for the playercar from a packet
    */
    void operator<<(sf::Packet &packet);

    void setMaxHealth(sf::Int16 health) { _MaxHealth = health; }

    void setHealth(sf::Int16 health) { _Health = health; }

    void applyKeyPress(sf::Uint8 keys);

    sf::Uint8 getPressedKeys() { return _PressedKeys; }

    void kill();

    bool isDying();

    PlayerCarIndex getPlayerCarIndex() {
        return _PlayerCarIndex;
    }

    void takeDamage(int damage);

private:
    void shoot();

    std::shared_ptr<Animation> _Animation;

    float _Energy;
    bool _InfiniteEnergy;
    sf::Uint16 _MaxEnergy, _Bulletdamage;
    PlayerCarIndex _PlayerCarIndex;

    sf::RectangleShape _AimLine;
    sf::Texture _CrosshairTexture;
    sf::Sprite _Crosshair;
    float _CrosshairSpeed;

    sf::Clock _AutoFireTimer;

    //Time it takes to reach maximum speed
    float _AccelerationTime;

    sf::Uint8 _PressedKeys;
};

