#pragma once

#include "GameObject/Boss/BossCar.h"
#include "GameObject/GameObjectFactory.h"

class Jet : public BossCar {
public:
    /*
        Jet boss
        @param texture Texture for the sprite
        @param bulletTexture Texture for all bullets the Jet shoots
    */
    Jet(unsigned int id, GameObjectManager &gom, int difficulty, int HP, sf::Texture &texture,
        sf::Texture &bulletTexture,
        sf::SoundBuffer &soundBufferEngine);

//    Jet(sf::Packet &packet, sf::Texture &texture, sf::Texture &bulletTexture, sf::SoundBuffer &soundBufferEngine, PlayerCar &player);

    ~Jet() {}

    /*
        See BossCar for details
    */
    void update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets);

    /*
        See BossCar for details
    */
    void render(sf::RenderWindow &window);

    void stopSounds();

    void init();

    void updateActions() {}

private:

    /*
        Sets the Jet to a random position off screen and sets the next position on the opposite site
    */
    void randomPosition();

    /*
        Changes to the next phase
    */
    void checkPhase();

    sf::SoundBuffer _EngineSoundBuffer;
    sf::Sound _EngineSound;
};
