#pragma once

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

#include "GameObject/PlayerCar.h"
#include "Animation/Explosion.h"

class GameObjectFactory
{
public:
	static void load();

	/*
		@param carSkin Index of the car texture
	*/
	static std::shared_ptr<PlayerCar> getPlayerCar(int carSkin);

	/*
		@param level
		@param diff
		@param hp
		@param soundEffects
		@param volume
		@return BossCar Constructed BossCar
	*/
	static std::shared_ptr<BossCar> getBossCar(int level, int diff, int hp, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume);
	
	/*
		Bullet that can be shot by any Car
		@param pos Starting position of the Bullet
		@param dir Angle at which the Bullet will fly
		@param speed Speed of the Bullet
		@param type Specifies the type of the Bullet, thus who shot it
		@param soundEffects Vector to register the sound of the shot in
		@param volume Volume of the shot sound
		@return Bullet
	*/
	static std::shared_ptr<Bullet> getBullet(sf::Vector2f pos, sf::Vector2f dir, int speed, GameObjectType type, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, float volume);
	
	/*
		Car that is controlled by the computer.
		@param hp Health of the car
		@param roadSpeed Speed of the road
		@param texture Texture that is going to be used for the sprite
	*/
	static std::shared_ptr<AICar> getAICar(int hp, int roadSpeed);

	/*
		@param pos Position of the Toolbox
	*/
	static std::shared_ptr<GameObject> getToolbox(sf::Vector2f pos);

	/*
		@param pos Position of the Canister
	*/
	static std::shared_ptr<GameObject> getCanister(sf::Vector2f pos);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<PlayerCar> getPlayerCar(std::istream& stream);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<BossCar> getBossCar(int level, std::istream& stream, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<Bullet> getBullet(std::istream& stream, GameObjectType type, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, float volume);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<AICar> getAICar(std::istream& stream);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<GameObject> getCanister(std::istream& stream);

	/*
		@param stream Stream to get the necessary data from
	*/
	static std::shared_ptr<GameObject> getToolbox(std::istream& stream);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<PlayerCar> getPlayerCar(sf::Packet& packet);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<BossCar> getBossCar(int level, sf::Packet& packet, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<Bullet> getBullet(sf::Packet& packet, GameObjectType type, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, float volume);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<AICar> getAICar(sf::Packet& packet);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<GameObject> getCanister(sf::Packet& packet);

	/*
		@param packet packet to get the necessary data from
	*/
	static std::shared_ptr<GameObject> getToolbox(sf::Packet& packet);

	static void scanStreamForGO(int level, std::istream& stream, std::vector<std::shared_ptr<GameObject>>& gos, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume);
	static void scanPacketForGO(int level, sf::Packet& packet, std::vector<std::shared_ptr<GameObject>>& gos, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume);

	static void setPlayerCarTextures(std::vector<std::shared_ptr<sf::Texture>>& carSkins) { _PlayerCarTextures() = carSkins; }

	static void setDeltaID(int deltaID) { _DeltaID = deltaID; }

private:
	static std::vector<std::shared_ptr<sf::Texture>>& _PlayerCarTextures();
	static std::vector<std::shared_ptr<sf::Texture>>& _AICarTextures();
	static std::vector<std::shared_ptr<sf::Texture>>& _BossCarTextures();
	static sf::Texture& _ToolboxTexture();
	static sf::Texture& _EnergyCanisterTexture();
	static sf::Texture& _BulletTexture();
	static sf::SoundBuffer& _AIShotSoundBuffer();
	static sf::SoundBuffer& _PlayerShotSoundBuffer();
	static sf::SoundBuffer&  _JetSoundBuffer();

	static sf::Uint32 _CurrentGameObjectID;
	static sf::Uint32 _DeltaID;
};
