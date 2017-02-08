//
// Created by henne on 08.02.17.
//

#pragma once

class Framework;

class SoundManager {
public:
    SoundManager(Framework &framework);

    void load();

    void update();

    void playHitSound(sf::Vector2f position);

    void playExplosionSound(sf::Vector2f position);

private:

    Framework &_FW;

    sf::Music _MenuMusic;

    std::vector<std::shared_ptr<sf::Music>> _LevelMusic;

    std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> _SoundEffects;

    sf::SoundBuffer _ExplosionSoundBuffer, _ImpactSoundBuffer;

    void updateVolumes();
};
