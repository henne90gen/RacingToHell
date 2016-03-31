#include "stdafx.h"
#include "GameObject/GameObjectFactory.h"


GameObjectFactory::GameObjectFactory()
{
}

GameObjectFactory::~GameObjectFactory()
{
	_PlayerCarTextures.clear();
	_AICarTextures.clear();
	_BossCarTextures.clear();
}

std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(int carSkin)
{
	std::shared_ptr<PlayerCar> player(new PlayerCar(carSkin, (*_PlayerCarTextures.at(carSkin))));
	return player;
}

std::shared_ptr<BossCar> GameObjectFactory::getBossCar(int level, int diff, int hp, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer expSB, float volume)
{
	switch (level)
	{
	case 0:
	{
		std::shared_ptr<Tank> boss(new Tank(diff, hp, (*_BossCarTextures[0]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}

	case 1:
	{
		std::shared_ptr<Mech> boss(new Mech(diff, hp, (*_BossCarTextures[2]), (*_BossCarTextures[3]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}
	case 2:
	{
		std::shared_ptr<Jet> boss(new Jet(diff, hp, (*_BossCarTextures[4]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, _JetSoundBuffer, volume));
		return boss;
		break;
	}
	case 3:
	{
		std::shared_ptr<Carrier> boss(new Carrier(diff, hp, (*_BossCarTextures[1]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}
	}
}

std::shared_ptr<Bullet> GameObjectFactory::getBullet(sf::Vector2f pos, sf::Vector2f dir, int speed, GameObjectType type, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, float volume)
{
	if (type == GameObjectType::BulletObjectAI || type == GameObjectType::BulletObjectBoss) {
		std::shared_ptr<Bullet> bullet(new Bullet(pos, dir, speed, type, _BulletTexture, soundEffects, _AIShotSoundBuffer, volume));
		return bullet;
	}
	else if (type == GameObjectType::BulletObjectPlayer) {
		std::shared_ptr<Bullet> bullet(new Bullet(pos, dir, speed, type, _BulletTexture, soundEffects, _PlayerShotSoundBuffer, volume));
		return bullet;
	}
}

std::shared_ptr<AICar> GameObjectFactory::getAICar(int hp, int roadSpeed)
{
	std::shared_ptr<AICar> car(new AICar(hp, roadSpeed, (*_AICarTextures.at(std::rand() % 7))));
	return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(sf::Vector2f pos)
{
	std::shared_ptr<GameObject> toolbox(new GameObject(pos, GameObjectType::Tools, _ToolboxTexture));
	return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(sf::Vector2f pos)
{
	std::shared_ptr<GameObject> canister(new GameObject(pos, GameObjectType::Canister, _EnergyCanisterTexture));
	return canister;
}



std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(std::istream& stream)
{
	std::shared_ptr<PlayerCar> player(new PlayerCar(stream, _PlayerCarTextures));
	return player;
}

std::shared_ptr<BossCar> GameObjectFactory::getBossCar(int level, std::istream& stream, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume)
{
	switch (level)
	{
	case 0:
	{
		std::shared_ptr<Tank> boss(new Tank(stream, (*_BossCarTextures[0]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}
	case 1:
	{
		std::shared_ptr<Mech> boss(new Mech(stream, (*_BossCarTextures[2]), (*_BossCarTextures[3]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}
	case 2:
	{
		std::shared_ptr<Jet> boss(new Jet(stream, (*_BossCarTextures[4]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, _JetSoundBuffer, volume));
		return boss;
		break;
	}
	case 3:
	{
		std::shared_ptr<Carrier> boss(new Carrier(stream, (*_BossCarTextures[1]), _BulletTexture, soundEffects, _AIShotSoundBuffer, expSB, volume));
		return boss;
		break;
	}
	}
}

std::shared_ptr<Bullet> GameObjectFactory::getBullet(std::istream& stream, GameObjectType type, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, float volume)
{
	if (type == GameObjectType::BulletObjectAI || type == GameObjectType::BulletObjectBoss) {
		std::shared_ptr<Bullet> bullet(new Bullet(stream, type, _BulletTexture, soundEffects, _AIShotSoundBuffer, volume));
		return bullet;
	}
	else if (type == GameObjectType::BulletObjectPlayer) {
		std::shared_ptr<Bullet> bullet(new Bullet(stream, type, _BulletTexture, soundEffects, _PlayerShotSoundBuffer, volume));
		return bullet;
	}
}

std::shared_ptr<AICar> GameObjectFactory::getAICar(std::istream& stream)
{
	std::shared_ptr<AICar> car(new AICar(stream, (*_AICarTextures.at(std::rand() % 7))));
	return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(std::istream& stream)
{
	std::shared_ptr<GameObject> toolbox(new GameObject(stream, GameObjectType::Tools, _ToolboxTexture));
	return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(std::istream& stream)
{
	std::shared_ptr<GameObject> canister(new GameObject(stream, GameObjectType::Canister, _EnergyCanisterTexture));
	return canister;
}

void GameObjectFactory::scanStreamForGOs(int level, std::istream& stream, std::vector<std::shared_ptr<GameObject>>& gos, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer& expSB, float volume)
{
	GameObjectType type;
	stream.read((char*)& type, sizeof(sf::Uint8));
	switch (type) {
	case GameObjectType::AI:
		gos.push_back(getAICar(stream));
		break;
	case GameObjectType::BulletObjectAI:
		gos.push_back(getBullet(stream, type, soundEffects, volume));
		break;
	case GameObjectType::BulletObjectBoss:
		gos.push_back(getBullet(stream, type, soundEffects, volume));
		break;
	case GameObjectType::BulletObjectPlayer:
		gos.push_back(getBullet(stream, type, soundEffects, volume));
		break;
	case GameObjectType::Tools:
		gos.push_back(getToolbox(stream));
		break;
	case GameObjectType::Canister:
		gos.push_back(getCanister(stream));
		break;
	case GameObjectType::Boss:
		gos.push_back(getPlayerCar(stream));
		break;
	case GameObjectType::Player:
		gos.push_back(getBossCar(level, stream, soundEffects, expSB, volume));
		break;
	}
}

void GameObjectFactory::load()
{
	// Loading Textures
	for (int i = 1; i <= 8; i++) {
		std::shared_ptr<sf::Texture> texture(new sf::Texture());
		(*texture).loadFromFile("Resources/Texture/TrafficCar/Traffic" + std::to_string(i) + ".png");
		_AICarTextures.push_back(texture);
	}

	std::string bossTextures[] =
	{
		"Tank",
		"Carrier",
		"mech-top", "mech-legs",
		"Jet"
	};

	for (int i = 0; i < sizeof(bossTextures) / sizeof(std::string); i++) {
		std::shared_ptr<sf::Texture>texture(new sf::Texture());
		texture->loadFromFile("Resources/Texture/BossCar/" + bossTextures[i] + ".png");
		_BossCarTextures.push_back(texture);
	}

	_BulletTexture.loadFromFile("Resources/Texture/Object/Bullet.png");
	_ToolboxTexture.loadFromFile("Resources/Texture/Object/toolbox.png");
	_EnergyCanisterTexture.loadFromFile("Resources/Texture/Object/canister.png");

	// Loading sounds
	_AIShotSoundBuffer.loadFromFile("Resources/Sound/shotAI.wav");
	_PlayerShotSoundBuffer.loadFromFile("Resources/Sound/shotPlayer.wav");
	_JetSoundBuffer.loadFromFile("Resources/Sound/jetengine.wav");
}