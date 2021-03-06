#pragma once

#include "Car.h"
#include "GameObject/GameObjectType.h"

class AICar : public Car {
public:
    /*
        Car that is controlled by the computer.
        @param hp Health of the car
        @param roadSpeed Speed of the road
        @param texture Texture that is going to be used for the sprite
    */
    AICar(unsigned int id, GameObjectManager &gom, int hp, int roadSpeed, sf::Texture &texture,
          sf::IntRect textureRect);

//    AICar(sf::Packet &packet, sf::Texture &texture, sf::IntRect textureRect);

    ~AICar() {}

    /*
        Renders the AICar to the specified RenderWindow
        @param window Window to draw to
    */
    void render(sf::RenderWindow &window);

    /*
        Handles events for AICar
        @param newEvent Event to be handled
    */
    void handleEvent(sf::Event &newEvent) {}

    /*
        Updates the AICar with the given frame time
        @param frameTime Time that has passed since the last update
        @param roadSpeed Speed of the road
    */
    void update(float frameTime, float roadSpeed);

    /*
        Current lane the car is on
        @return int Number of the lane
    */
    sf::Uint8 getLane() { return _Lane; }

    /*
        Choses a new lane for the car randomly
    */
    void switchLaneRandomly();

    /*
        Calculates the corresponding x position to the lane
        @return x-position
    */
    inline int laneToX();

    /*
        Writes the necessary data for a GameObject to a packet
    */
    void operator>>(sf::Packet &packet);

    /*
        Reads the necessary data for a GameObject from a packet
    */
    void operator<<(sf::Packet &packet);

private:
    sf::RectangleShape _HealthBar;
    sf::RectangleShape _HealthBarFrame;

    sf::Uint8 _Lane;

    void init();
};

