
#pragma once

#include "UserInterface/Screen/GameScreen.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectType.h"
#include "GameObject/AICar.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/Bullet.h"
#include "GameObject/GameObjectFactory.h"

#include "GameObject/Boss/BossCar.h"
#include "GameObject/Boss/Tank.h"
#include "GameObject/Boss/Carrier.h"
#include "GameObject/Boss/Mech.h"
#include "GameObject/Boss/Jet.h"

#include "Animation/Explosion.h"

class GameObjectManager {
public:
    /**
     * GameObjectManager is the manager class for all GameObjects.
     * It handles their creation and deletion, as well as updating and rendering them
     */
    GameObjectManager(Framework &framework);

    ~GameObjectManager();

    /**
     * Updates all the GameObjects with the given frame time
     * @param frameTime Time that has passed since the last update
     * @param level Level
     * @param roadSpeed Speed of the road
     */
    void update(float frameTime);

    /**
     * @return GameObject* Pointer to the PlayerCar
     */
    std::shared_ptr<PlayerCar> getPlayerCar() { return _Player; }

    /**
     * @return GameObject* Pointer to the BossCar
     */
    std::shared_ptr<BossCar> getBossCar() { return _Boss; }

    /**
     * @return bool True if the boss is still alive
     */
    bool bossIsDead();

    /**
     * Sets all the necessary variables for a boss fight
     */
    void enterBossFight();

    /**
     * Resets the GameObjectManager to a state where a new game can begin
     * @param selectedCar Index of the car that was selected by the player
     */
    void resetGameObjects();

    /**
     * Empties the screen
     * @return bool True as soon as there are no GamObjects other than the player on the screen
     */
    bool emptyScreen();

    /**
     * Loads textures and sounds for all GameObjects
     */
    void load();

    /**
     * Plays sounds for every GameObject
     */
    void playSounds();

    /**
     * Stops all sounds from playing
     */
    void stopSounds();

    void nextPlayerCar();

    void previousPlayerCar();

    std::vector<std::shared_ptr<GameObject>> &getPickupItems() { return _PickupItems; }

    std::vector<std::shared_ptr<AICar>> &getCars() { return _Cars; }

    std::vector<std::shared_ptr<Bullet>> &getBullets() { return _Bullets; }

    std::vector<std::shared_ptr<Animation>> &getAnimations() { return _Animations; }

    float getCarFrequency() { return _CarFrequency; }

    float getBulletFrequency() { return _BulletFrequency; }

    float getCanisterFrequency() { return _CanisterFrequency; }

    float getToolboxFrequency() { return _ToolboxFrequency; }

private:
    Framework &_FW;

    std::vector<std::shared_ptr<GameObject>> _PickupItems;
    std::vector<std::shared_ptr<AICar>> _Cars;
    std::vector<std::shared_ptr<Bullet>> _Bullets;
    std::shared_ptr<PlayerCar> _Player;
    std::shared_ptr<BossCar> _Boss;

    std::vector<std::shared_ptr<Animation>> _Animations;

    sf::Texture _ExplosionTexture;
    // TODO remove sound buffers and sound vector at some point
    sf::SoundBuffer _ExplosionSoundBuffer, _ImpactSoundBuffer;
    std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> _SoundEffects;


    float _CarFrequency, _BulletFrequency, _CanisterFrequency, _ToolboxFrequency, _TimePassedCar, _TimePassedBullet, _TimePassedCanister, _TimePassedToolbox;

    int _PlayerBulletSpeed, _AIBulletSpeed;
    bool _AboutToLevelUp, _BossFight;

//    /**
//     * Plays a shot sound depending on the type of the firing entity
//     * @param go Type of the GameObject that is emitting the shot sound
//     * @param position Position of the Soundobject
//     */
//    void playShotSound(GameObjectType go, sf::Vector2f position = sf::Vector2f(0.f, 0.f));

    /**
     * Spawns a new AICar on a random lane
     * @param level Levelnumber
     * @param roadSpeed Speed of the road
     */
    void spawnAICar(float frameTime);

    /**
     * Makes a random AICar shoot a bullet at the player
     */
    void spawnBullet(float frameTime);

    void spawnToolbox(float frameTime);

    void spawnCanister(float frameTime);

    template <typename T>
    void deleteOffScreenObjects(T &goList);

    /**
     * Deletes a GameObject from the vector of GameObjects
     * @param id Index of the GameObject that will be deleted
     */
    template<typename T>
    void deleteObject(T &arr, unsigned int id) {
        arr.erase(arr.begin() + id);
    }

    /**
     * Sets all frequencies dependig on the selected difficulty
     */
    void calculateAllFrequencies();

    void calculateAiCarFrequency();

    void calculateBulletFrequency();

    void calculateCanisterFrequency();

    void calculateToolboxFrequency();

    void spawnObjects(float frameTime);

    void checkForCollisions(float frameTime);

    void deleteAllOffScreenObjects();

    void checkPlayerForCollisions(float frameTime);

    void deleteDestroyedCars();
};
