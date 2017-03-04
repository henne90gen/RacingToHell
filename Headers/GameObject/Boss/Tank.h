#pragma once

#include "GameObject/Boss/BossCar.h"
#include "GameObject/GameObjectFactory.h"

class Tank : public BossCar {
public:
    /*
        Tank boss
        @param texture Texture for the sprite
        @param bulletTexture Texture for all bullets the tank shoots
    */
    Tank(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &Texture,
         sf::Texture &BulletTexture);

//    Tank(sf::Packet &packet, sf::Texture &Texture, sf::Texture &BulletTexture, PlayerCar &player);

    ~Tank() {}

    void render(sf::RenderWindow &RenderWindow);

    void update(float FrameTime);

    void init();

    void updateActions();

private:
    sf::Texture _GunTexture;
    sf::Sprite _GunSprite;

    bool _MovementSwitch;
};

