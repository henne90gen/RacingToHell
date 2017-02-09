//
// Created by henne on 08.02.17.
//

#pragma once

#include <GameObject/GameObjectType.h>

class Framework;

class SoundManager {
public:
    SoundManager(Framework &framework);

    /**
     * Loads all sound effects and music files into memory.
     */
    void load();

    /**
     * Updates volume and which sounds should be played.
     */
    void update();

    /**
     * Plays a sound effect for shooting bullets
     * @param shooter Who shot the bullet? (Player/AI)
     * @param position Where is the sound coming from?
     */
    void playShotSound(GameObjectType shooter, sf::Vector2f position);

    /**
     * Plays a sound effect for being hit by a bullet
     * @param position Where is the sound coming from?
     */
    void playHitSound(sf::Vector2f position);

    /**
     * Plays a sound effect for explosions
     * @param position Where is the sound coming from?
     */
    void playExplosionSound(sf::Vector2f position);

    /**
     * Plays a sound effect for the game over menu
     */
    void playGameOverSound();

    /**
     * Switches the level music to the next track
     */
    void nextLevel();

    /**
     * @return Size of sound effects vector
     */
    unsigned long getNumSoundEffects() { return _SoundEffects.size(); }

    /**
     * @return Index of the current level music track
     */
    unsigned long getLevelMusicIndex();

private:
    Framework &_FW;

    const std::string _SoundPath = "Resources/Sound/";

    // Music is a pointer because sf::Music can't be copied around
    std::shared_ptr<sf::Music> _MenuMusic, _LevelMusic;

    sf::SoundBuffer _GameOverSoundBuffer;

    std::vector<std::shared_ptr<sf::Music>> _AllLevelMusic;

    std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> _SoundEffects;

    sf::SoundBuffer _ExplosionSoundBuffer, _ImpactSoundBuffer, _AIShotSoundBuffer, _PlayerShotSoundBuffer;

    void updateVolumes();

    /**
     * Checks GameState for menu GameStates
     * @return true, if menu music should be played
     */
    bool isInMenu();

    void updateMenu();

    /**
     * @return true, if level music should be played
     */
    bool isInLevel();

    void updateLevel();

    /**
     * Checks the following things for sound effects.
     * Have they started playing yet?
     * Are they still playing?
     * Are they done playing?
     */
    void updateSoundEffects();
};
