//
// Created by henne on 08.02.17.
//

#include "stdafx.h"
#include "Framework/SoundManager.h"
#include "Framework/Framework.h"

SoundManager::SoundManager(Framework &framework) : _FW(framework) {
    _MenuMusic = std::make_shared<sf::Music>();
    _LevelMusic = std::make_shared<sf::Music>();
}

void SoundManager::load() {
    std::cout << "Loading music and sound..." << std::endl;

//    Load menu music
    if (!_MenuMusic->openFromFile(_SoundPath + "Music/menu1.ogg")) {
        std::cout << "Couldn't load music for menu" << std::endl;
    }

//    Load level music
    for (int i = 1; i <= 4; i++) {
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        if (!(*music).openFromFile(_SoundPath + "Music/level" + std::to_string(i) + ".ogg")) {
            std::cout << "Couldn't load music for level " << i << std::endl;
        }
        _AllLevelMusic.push_back(music);
    }
    _LevelMusic = _AllLevelMusic.at(0);

//    Load sound effects
    if (!_ExplosionSoundBuffer.loadFromFile(_SoundPath + "explosion.wav")) {
        std::cout << "Couldn't load Explosion sound." << std::endl;
    }
    if (!_ImpactSoundBuffer.loadFromFile(_SoundPath + "impact.wav")) {
        std::cout << "Couldn't load Impact sound." << std::endl;
    }
    if (!_AIShotSoundBuffer.loadFromFile(_SoundPath + "shotAI.wav")) {
        std::cout << "Couldn't load AIShot sound." << std::endl;
    }
    if (!_PlayerShotSoundBuffer.loadFromFile(_SoundPath + "shotPlayer.wav")) {
        std::cout << "Couldn't load PlayerShot sound." << std::endl;
    }
    if (!_GameOverSoundBuffer.loadFromFile(_SoundPath + "gameOver.wav")) {
        std::cout << "Couldn't load GameOver sound." << std::endl;
    }
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
    _MenuMusic->setVolume(volume * 9);
    _LevelMusic->setVolume(volume * 7);

//    _GameObjectContainer.setVolume((float) (volume * 2.7));
//    _LevelUpScreen.setVolume(volume * 100);
//    _GameOverScreen.setVolume(volume * 10);
}

void SoundManager::updateMenu() {
    if (isInMenu()) {
        if (_MenuMusic->getStatus() != sf::Music::Playing) {
            _MenuMusic->play();
        }
    }
}

void SoundManager::updateLevel() {
    if (isInLevel()) {
        if (_LevelMusic->getStatus() != sf::Music::Playing) {
            _LevelMusic->play();
        }
    }
}

void SoundManager::updateSoundEffects() {
    for (unsigned int i = 0; i < _SoundEffects.size(); i++) {
        if (_SoundEffects[i].first->getStatus() == sf::Music::Stopped ||
            _SoundEffects[i].first->getStatus() == sf::Music::Paused) {
            if (_SoundEffects[i].second) {
                _SoundEffects.erase(_SoundEffects.begin() + i);
            } else {
                _SoundEffects[i].first->play();
                _SoundEffects[i].second = true;
            }
        }
    }
}

// TODO balance audio levels
void SoundManager::playShotSound(GameObjectType shooter, sf::Vector2f position) {
    std::shared_ptr<sf::Sound> shotSound = std::make_shared<sf::Sound>();
    if (shooter == GameObjectType::AI) {
        shotSound->setBuffer(_AIShotSoundBuffer);
    } else if (shooter == GameObjectType::Player) {
        shotSound->setBuffer(_PlayerShotSoundBuffer);
    }
    shotSound->setVolume(_FW.getOptionsManager().getVolume() * 2);
    shotSound->setPosition(position.x, position.y, 0);
    shotSound->setMinDistance(650.0f);
    shotSound->setAttenuation(2.0f);
    _SoundEffects.push_back(std::make_pair(shotSound, 0));
}

// TODO balance audio levels
void SoundManager::playHitSound(sf::Vector2f position) {
    std::shared_ptr<sf::Sound> impactSound = std::make_shared<sf::Sound>();
    impactSound->setBuffer(_ImpactSoundBuffer);
    impactSound->setVolume(_FW.getOptionsManager().getVolume() * 5.5f);
    impactSound->setPosition(sf::Vector3f(position.x, 0, position.y));
    impactSound->setMinDistance(650.0f);
    impactSound->setAttenuation(2.0f);
    _SoundEffects.push_back(std::make_pair(impactSound, false));
}

// TODO balance audio levels
void SoundManager::playExplosionSound(sf::Vector2f position) {
    std::shared_ptr<sf::Sound> explosionSound = std::make_shared<sf::Sound>();
    explosionSound->setBuffer(_ExplosionSoundBuffer);
    explosionSound->setPosition(position.x, 0.0f, position.y);
    explosionSound->setMinDistance(500.0f);
    explosionSound->setAttenuation(4.0f);
    explosionSound->setVolume(_FW.getOptionsManager().getVolume() * 5.0f);
    _SoundEffects.push_back(std::make_pair(explosionSound, false));
}

void SoundManager::playGameOverSound() {
    std::shared_ptr<sf::Sound> gameOverSound = std::make_shared<sf::Sound>();
    gameOverSound->setBuffer(_GameOverSoundBuffer);
    gameOverSound->setVolume(_FW.getOptionsManager().getVolume());
    _SoundEffects.push_back(std::make_pair(gameOverSound, false));
}

bool SoundManager::isInMenu() {
    GameState gs = _FW.getCurrentGameState();
    return gs == GameState::About || gs == GameState::MainMenu || gs == GameState::Options || gs == GameState::Pause ||
           gs == GameState::Highscores;
}

bool SoundManager::isInLevel() {
	
	return _FW.getCurrentGameState() == GameState::Running;
}

unsigned long SoundManager::getLevelMusicIndex() {
    return (unsigned long) ((_FW.getLevelManager().getLevel() - 1) % 4);
}

void SoundManager::nextLevel() {
    for (unsigned long i = 0; i < _AllLevelMusic.size(); i++) {
        if (i == getLevelMusicIndex()) {
            _LevelMusic = _AllLevelMusic.at(i);
        }
    }
}
