#pragma once

#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectType.h"
#include "GameObject/AICar.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/Bullet.h"

#include "GameObject/Boss/BossCar.h"
#include "GameObject/Boss/Tank.h"
#include "GameObject/Boss/Carrier.h"
#include "GameObject/Boss/Mech.h"
#include "GameObject/Boss/Jet.h"

#include "Animation/Explosion.h"

#include "Multiplayer\NetworkHandle.h"

class MPGameObjectContainer
{
public:
	/*
	GameObjectContainer is the manager class for all GameObjects.
	It handles their creation and deletion, as well as updating and rendering them
	*/
	MPGameObjectContainer();
	~MPGameObjectContainer();

	/*
	Renders all the GameObjects to the screen
	@param window RenderWindow to draw to
	@param renderCrosshair Determines whether various aiming utilities of the PlayerCar (crosshair and aim line) are going to be rendered
	*/
	void render(sf::RenderWindow& window, bool renderCrosshair);

	/*
	Handles events and delegates them to the correct GameObjects
	@param newEvent Event to be handled
	*/
	void handleEvent(sf::Event& newEvent);

	/*
	Updates all the GameObjects with the given frame time
	@param frameTime Time that has passed since the last update
	@param level Level
	@param roadSpeed Speed of the road
	*/
	void update(float frameTime, int roadSpeed);

	/*
		@return GameObject& Reference to the PlayerCar
	*/
	GameObject& getPlayerCar() { return *_GameObjects.at(0); }

	/*
		@return GameObject& Reference to the PlayerCar
	*/
	GameObject& getPlayer2Car() { return *_GameObjects.at(1); }

	/*
		@return GameObject& Reference to the BossCar
	*/
	GameObject& getBossCar() { return *_GameObjects.at(1); }

	/*
		@return bool True if the player is still alive
	*/
	bool playerIsAlive();

	/*
	@return bool True if the boss is still alive
	*/
	bool bossIsDead();

	/*
	Sets all the necessary variables for a boss fight
	*/
	void enterBossFight();

	/*
	Resets the GameObjectManager to a state where a new game can begin
	@param selectedCar Index of the car that was selected by the player
	*/
	void resetGameObjects(int selectedCar);

	/*
	Empties the screen
	@return bool True as soon as there are no GamObjects other than the player on the screen
	*/
	bool emptyScreen();

	/*
	Loads textures and sounds for all GameObjects
	*/
	void load();

	/*
	Plays sounds for every GameObject
	*/
	void playSounds();

	/*
	Stops all sounds from playing
	*/
	void stopSounds();

	/*
	Sets the Volume for all GamObjects
	@param volume New volume that's being applied
	*/
	void setVolume(float volume) { _Volume = volume; }

	/*
	Returns and resets the car score
	@return int Score the player has accumulated since the last call to this method
	*/
	int getCarScore() { int result = _CarScore; _CarScore = 0; return result; }

	/*
	Passes in the car skin texture so that the right one can be applied to the player car
	@param carSkins Vector with all car skin textures
	*/
	void setCarSkins(std::vector<std::shared_ptr<sf::Texture>>& carSkins);

	/*
	Sets the current level
	@param lvl Level
	*/
	void setLevel(int lvl) { _Level = lvl; }

	/*
	Sets selected diffculty
	@param dif Difficulty
	*/
	void setDifficulty(int dif) { _Difficulty = dif; }

	void setNetworkRelation(NetworkRelation relation) { _Relation = relation; }

	void addGameObject(std::shared_ptr<GameObject> newGO);

	void handleIncomingPackets(std::vector<sf::Packet>& packets, sf::Uint32 tick, int delay);
	void handleOutgoingPackets(std::vector<std::pair<NetworkCommunication, sf::Packet>>& packets);
private:
	std::mutex _Mutex;
	NetworkRelation _Relation;
	sf::Clock _SendTimer;

	std::vector<std::shared_ptr<GameObject>> _GameObjects;
	std::vector<std::pair<NetworkCommunication, std::shared_ptr<GameObject>>> _SendObjects;
	std::vector<std::shared_ptr<Animation>> _Animations;

	sf::Texture _ExplosionTexture;
	sf::SoundBuffer _ExplosionSoundBuffer;
	sf::SoundBuffer _ImpactSoundBuffer;

	std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>> _SoundEffects;
	float _Volume;

	float _CarFrequency, _BulletFrequency, _CanisterFrequency, _ToolboxFrequency, _TimePassedCar, _TimePassedBullet, _TimePassedCanister, _TimePassedToolbox;

	// Score the player accumulates for shooting other cars
	int _CarScore, _Level, _Difficulty;
	int _PlayerBulletSpeed, _AIBulletSpeed;
	bool _PlayerAlive, _AboutToLevelUp, _BossFight;

	/*
	Plays a shot sound depending on the type of the firing entity
	@param go Type of the GameObject that is emitting the shot sound
	@param position Position of the Soundobject
	*/
	void playShotSound(GameObjectType go, sf::Vector2f position = sf::Vector2f(0.f, 0.f));

	/*
	Spawns a new AICar on a random lane
	@param level Levelnumber
	@param roadSpeed Speed of the road
	*/
	void spawnAICar(int roadSpeed);

	/*
	Makes a random AICar shoot a bullet at the player
	*/
	void spawnBullet();

	/*
	Deletes a GameObject from the vector of GameObjects
	@param id Index of the GameObject that will be deleted
	*/
	void deleteGameObject(unsigned int id, bool sendDeletion);

	/*
	calculates the hp of a AIcar
	@return HP of the AICar
	*/
	int getAiHP();

	/*
	Sets all frequencies dependig on the selected difficulty
	*/
	void setAllFrequencies();

	void setAiCarFrequency();
	void setBulletFrequency();
	void setCanisterFrequency();
	void setToolboxFrequency();

	/*
	@retun Boss HP at given level
	*/
	int getBossHP();

	void playHitSound(sf::Vector2f position);
};