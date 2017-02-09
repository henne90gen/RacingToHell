//
// Created by henne on 08.02.17.
//

#pragma once

#include <GameObject/GameObjectType.h>

class Framework;

class SoundManager {
public:
    SoundManager(Framework &framework);

    void load();

    void update();

    void playShotSound(GameObjectType shooter, sf::Vector2f position);

    void playHitSound(sf::Vector2f position);

    void playExplosionSound(sf::Vector2f position);

    void nextLevel();

    unsigned long getNumSoundEffects() { return _SoundEffects.size(); }

    unsigned long getLevelMusicIndex();

private:

    Framework &_FW;

    // Music is a pointer because sf::Music can't be copied around
    std::shared_ptr<sf::Music> _MenuMusic, _LevelMusic;

    std::vector<std::shared_ptr<sf::Music>> _AllLevelMusic;

    std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> _SoundEffects;

    sf::SoundBuffer _ExplosionSoundBuffer, _ImpactSoundBuffer, _AIShotSoundBuffer, _PlayerShotSoundBuffer;

    void updateVolumes();

    bool isInMenu();

    void updateMenu();

    bool isInLevel();

    void updateLevel();

    void updateSoundEffects();
};
