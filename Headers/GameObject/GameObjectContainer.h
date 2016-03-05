#pragma once

#include "GameObject/GameObject.h"
#include "GameObject/GameObjects.h"
#include "GameObject/AICar.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/BossCar.h"
#include "GameObject/Bullet.h"
#include "GameObject/EnergyCanister.h"
#include "GameObject/Toolbox.h"

class GameObjectContainer
{
public:
	GameObjectContainer();
	~GameObjectContainer();

	void update(float FrameTime, int Difficulty, int RoadSpeed);
	void render(sf::RenderWindow& RenderWindow, bool renderCrosshair);
	void handleEvents(sf::Event& Event);

	PlayerCar* getPlayerCar() { return dynamic_cast<PlayerCar*>(_GameObjects.at(0)); }
	bool playerIsAlive() { return _PlayerAlive; }
	bool bossIsDead();

	void enterBossFight(bool entering);

	void resetGameObjects(int SelectedCar);
	bool emptyScreen();

	void load();

	void playSounds();
	void setVolume(float Volume) { _Volume = Volume; }

	int getCarScore() { int result = _CarScore; _CarScore = 0; return result; }

	void setCarSkins(std::vector<sf::Texture*>& CarSkins);

private:
	std::vector<GameObject*> _GameObjects;
	std::vector<sf::Texture*> _PlayerCarTextures, _AICarTextures;
	std::vector<std::pair<sf::Texture*, sf::Texture*>> _BossCarTextures;
	std::pair<sf::Texture*, sf::Texture*> /*Temporary solution for boss texture*/ _BossCarTexture;
	sf::Texture _ToolboxTexture, _EnergyCanisterTexture, _BulletTexture;

	sf::SoundBuffer _AIShotSoundBuffer, _PlayerShotSoundBuffer;
	std::vector<std::pair<sf::Sound*, bool>> _SoundEffects;
	float _Volume;

	float _CarFrequency, _BulletFrequency, _CanisterFrequency, _ToolboxFrequency, _TimePassedCar, _TimePassedBullet, _TimePassedCanister, _TimePassedToolbox;
	int _CarScore, _PlayerBulletSpeed, _AIBulletSpeed;
	bool _PlayerAlive, _AboutToLevelUp, _BossFight;

	void playShotSound(GameObjects go);
	void spawnAICar(int Difficulty, int RoadSpeed);
	void spawnBullet();
	void deleteObject(unsigned int id);
};