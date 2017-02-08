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

    updateMenu();

    updateLevel();

    updateSoundEffects();
}

void SoundManager::updateVolumes() {
    float volume = _FW.getOptionsManager().getVolume() / 10.0f;

    sf::Listener::setGlobalVolume(volume * 7);
    _MenuMusic.setVolume(volume * 9);
//    _LevelManager.setVolume(volume * 7);
//    _GameObjectContainer.setVolume((float) (volume * 2.7));
//    _LevelUpScreen.setVolume(volume * 100);
//    _GameOverScreen.setVolume(volume * 10);
}

void SoundManager::updateMenu() {
    if (isInMenu()) {
        if (_MenuMusic.getStatus() != sf::SoundSource::Playing) {
            _MenuMusic.play();
        }
    } else {
        if (_MenuMusic.getStatus() == sf::SoundSource::Playing) {
            _MenuMusic.stop();
        }
    }
}

void SoundManager::updateLevel() {
    if (isInLevel()) {
        for (unsigned long i = 0; i < _LevelMusic.size(); i++) {
            if (i == getLevelMusicIndex() && _LevelMusic.at(i)->getStatus() != sf::SoundSource::Playing) {
                _LevelMusic.at(i)->play();
            } else if (_LevelMusic.at(i)->getStatus() == sf::SoundSource::Playing) {
                _LevelMusic.at(i)->stop();
            }
        }
    } else {
        for (unsigned long i = 0; i < _LevelMusic.size(); i++) {
            if (_LevelMusic.at(i)->getStatus() == sf::SoundSource::Playing) {
                _LevelMusic.at(i)->stop();
            }
        }
    }
}

void SoundManager::updateSoundEffects() {
    for (unsigned int i = 0; i < _SoundEffects.size(); i++) {
        if (_SoundEffects[i].first->getStatus() == sf::SoundSource::Stopped ||
            _SoundEffects[i].first->getStatus() == sf::SoundSource::Paused) {
            if (_SoundEffects[i].second) {
                _SoundEffects.erase(_SoundEffects.begin() + i);
            } else {
                _SoundEffects[i].first->play();
                _SoundEffects[i].second = true;
            }
        }
    }
}

void SoundManager::playShotSound(sf::Vector2f position) {

}

void SoundManager::playHitSound(sf::Vector2f position) {
    std::shared_ptr<sf::Sound> impactSound = std::make_shared<sf::Sound>();
    impactSound->setBuffer(_ImpactSoundBuffer);
    impactSound->setVolume(_FW.getOptionsManager().getVolume() * 5.5f);
    impactSound->setPosition(sf::Vector3f(position.x, 0, position.y));
    impactSound->setMinDistance(650.0f);
    impactSound->setAttenuation(2.0f);
    _SoundEffects.push_back(std::make_pair(impactSound, false));
}

void SoundManager::playExplosionSound(sf::Vector2f position) {

}

bool SoundManager::isInMenu() {
    GameState gs = _FW.getCurrentGameState();
    return gs == GameState::About || gs == GameState::MainMenu || gs == GameState::Options || gs == GameState::Pause ||
           gs == GameState::Highscores;
}

bool SoundManager::isInLevel() {
    GameState gs = _FW.getCurrentGameState();
    return gs == GameState::Running;
}

unsigned long SoundManager::getLevelMusicIndex() {
    return (unsigned long) (_FW.getLevelManager().getLevel() % 4);
}
