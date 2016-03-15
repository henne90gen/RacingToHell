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

#include "Animation/Explosion.h"

class GameObjectContainer
{
public:
	GameObjectContainer();
	~GameObjectContainer();

	void update(float FrameTime, int Difficulty, int RoadSpeed);
	void render(sf::RenderWindow& Window, bool renderCrosshair);
	void handleEvents(sf::Event& Event);

	GameObject* getPlayerCar() { return _GameObjects.at(0); }
	GameObject* getBossCar() { return _GameObjects.at(1); }
	bool playerIsAlive();
	bool bossIsDead();

	void enterBossFight();

	void resetGameObjects(int SelectedCar);
	bool emptyScreen();

	void load();

	void playSounds();
	void setVolume(float Volume) { _Volume = Volume; }

	int getCarScore() { int result = _CarScore; _CarScore = 0; return result; }

	void setCarSkins(std::vector<sf::Texture*>& CarSkins);

private:
	std::vector<GameObject*> _GameObjects;
	std::vector<Animation*> _Animations;
	std::vector<sf::Texture*> _PlayerCarTextures, _AICarTextures, _BossCarTextures;
	sf::Texture _ToolboxTexture, _EnergyCanisterTexture, _BulletTexture, _ExplosionTexture;

	sf::SoundBuffer _AIShotSoundBuffer, _PlayerShotSoundBuffer;
	std::vector<std::pair<sf::Sound*, bool>> _SoundEffects;
	float _Volume;

	float _CarFrequency, _BulletFrequency, _CanisterFrequency, _ToolboxFrequency, _TimePassedCar, _TimePassedBullet, _TimePassedCanister, _TimePassedToolbox;
	int _CarScore, _PlayerBulletSpeed, _AIBulletSpeed;
	bool _PlayerAlive, _AboutToLevelUp, _BossFight;

	void playShotSound(GameObjectType go);
	void spawnAICar(int Difficulty, int RoadSpeed);
	void spawnBullet();
	void deleteObject(unsigned int id);
};