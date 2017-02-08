//
// Created by henne on 08.02.17.
//

#include "stdafx.h"
#include "Framework/SoundManager.h"
#include "Framework/Framework.h"

SoundManager::SoundManager(Framework &framework) : _FW(framework) {

}

void SoundManager::load() {
    std::cout << "Loading music and sound..." << std::endl;

//    Load menu music
    if (!_MenuMusic.openFromFile("Resources/Sound/Music/menu1.ogg")) {
        std::cout << "Couldn't load music for menu" << std::endl;
    }

//    Load level music
    for (int i = 1; i <= 4; i++) {
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        if (!(*music).openFromFile("Resources/Sound/Music/level" + std::to_string(i) + ".ogg")) {
            std::cout << "Couldn't load music for level " << i << std::endl;
        }
        _LevelMusic.push_back(music);
    }

    _ExplosionSoundBuffer.loadFromFile("Resources/Sound/explosion.wav");
    _ImpactSoundBuffer.loadFromFile("Resources/Sound/impact.wav");
}

void SoundManager::update() {
    updateVolumes();

    for (unsigned int i = 0; i < _SoundEffects.size(); i++) {
        if (_SoundEffects[i].first->getStatus() == sf::Sound::Stopped ||
            _SoundEffects[i].first->getStatus() == sf::Sound::Paused) {
            if (_SoundEffects[i].second) {
                _SoundEffects.erase(_SoundEffects.begin() + i);
            } else {
                _SoundEffects[i].first->play();
                _SoundEffects[i].second = true;
            }
        }
    }
}

void SoundManager::playHitSound(sf::Vector2f position) {
    std::shared_ptr<sf::Sound> ImpactSound = std::make_shared<sf::Sound>();
    ImpactSound->setBuffer(_ImpactSoundBuffer);
    ImpactSound->setVolume(_FW.getOptionsManager().getVolume() * 5.5f);
    ImpactSound->setPosition(sf::Vector3f(position.x, 0, position.y));
    ImpactSound->setMinDistance(650.0f);
    ImpactSound->setAttenuation(2.0f);
    _SoundEffects.push_back(std::make_pair(ImpactSound, false));
}

void SoundManager::updateVolumes() {

    float volume = _FW.getOptionsManager().getVolume();
    sf::Listener::setGlobalVolume(volume * 7);
    _MenuMusic.setVolume(volume * 9);
//    _LevelManager.setVolume(volume * 7);
//    _GameObjectContainer.setVolume((float) (volume * 2.7));
//    _LevelUpScreen.setVolume(volume * 100);
//    _GameOverScreen.setVolume(volume * 10);
}

void SoundManager::playExplosionSound(sf::Vector2f position) {

}
