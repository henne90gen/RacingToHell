#pragma once

#include "GameObject.h"
#include "GameObjects.h"
#include "AICar.h"
#include "PlayerCar.h"
#include "Bullet.h"
#include "EnergyCanister.h"
#include "Toolbox.h"

class GameObjectContainer
{
public:
	GameObjectContainer();
	~GameObjectContainer();

	void update(float FrameTime, int Difficulty, int RoadSpeed);
	void render(sf::RenderWindow& RenderWindow);
	void handleEvents(sf::Event& Event);

	PlayerCar* getPlayerCar() { return dynamic_cast<PlayerCar*>(_GameObjects.at(0)); }
	bool playerIsAlive() { return _PlayerAlive; }

	void resetGameObjects(int SelectedCar);

	int getCarScore() { int result = _CarScore; _CarScore = 0; return result; }

private:
	std::vector<GameObject*> _GameObjects;
	std::vector<sf::Texture*> _PlayerCarTextures, _AICarTextures;
	sf::Texture _ToolboxTexture, _EnergyCanisterTexture, _BulletTexture;

	float _CarFrequency, _BulletFrequency, _CanisterFrequency, _ToolboxFrequency, _TimePassedCar, _TimePassedBullet, _TimePassedCanister, _TimePassedToolbox;
	int _CarScore, _PlayerBulletSpeed, _AIBulletSpeed;
	bool _PlayerAlive;

	void spawnAICar(int Difficulty, int RoadSpeed);
	void spawnBullet();
	void deleteObject(unsigned int id);
};