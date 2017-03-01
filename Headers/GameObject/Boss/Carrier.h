#pragma once

#include "GameObject/Boss/BossCar.h"
#include "GameObject/GameObjectFactory.h"

class Carrier : public BossCar {
public:
    /*
        Carrier boss
        @param texture Texture for the sprite
        @param bulletTexture Texture for all bullets the boss shoots
    */
    Carrier(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &texture,
            sf::Texture &bulletTexture);

//    Carrier(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, PlayerCar &player);

    virtual ~Carrier() {}

    /*
        See BossCar for details
    */
    void update(float FrameTime, int RoadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets);

    /*
        See BossCar for details
    */
    void render(sf::RenderWindow &RenderWindow);

    void init();

    void updateActions() {}

private:
    sf::Texture _GunTexture;
    sf::Sprite _GunSprite;

    float _Radius;

    bool _MovementSwitchLeftRight, _MovementSwitchUpDown, _SwitchSides;
    sf::Clock _SwitchSidesClock;
    float _SwitchSideTime;

    void checkPhase();
};
