#include "stdafx.h"
#include "GameObject/GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(400) {}

GameObjectContainer::~GameObjectContainer()
{
	_GameObjects.clear();
	_Cars.clear();
	_Bullets.clear();
}

void GameObjectContainer::update(float FrameTime, int RoadSpeed)
{
	// Update Animations
	for (int i = 0; i < _Animations.size(); i++) {
		if (_Animations[i]->getAnimationState() == Animation::AnimationState::Stop) {
			_Animations.erase(_Animations.begin() + i);
			i--;
		}
		else {
			_Animations[i]->update(FrameTime);
		}
	}

	if (_PlayerAlive) {
		_Player->update(FrameTime, RoadSpeed);
	}
	if (_PlayerAlive && _Player->getHealth() <= 0) {
		_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(_Player->getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
		_PlayerAlive = false;
	}

	// Collision player with cars
	for (unsigned int i = 0; i < _Cars.size(); i++)
	{
		_Cars.at(i)->update(FrameTime, RoadSpeed);
		if (_PlayerAlive && _Player->checkForCollision(*_Cars.at(i))) {
			if (_GameMode == GameMode::Invincible)
			{
				std::shared_ptr<Explosion> newExplosion(new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume));
				_Animations.push_back(newExplosion);
				deleteObject(_Cars, i);
				i--;
			}
			else
			{
				_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(_Player->getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
				_PlayerAlive = false;
			}

		}
	}
	// Collision player with bullets
	for (unsigned int i = 0; i < _Bullets.size(); i++)
	{
		_Bullets[i]->update(FrameTime, RoadSpeed);
		if (_PlayerAlive && _Player->checkForCollision(*_Bullets.at(i))) {
			switch (_Bullets[i]->getType())
			{
			case GameObjectType::BulletObjectAI:
				if (_GameMode != GameMode::Invincible)
				{
					_Player->takeDamage(5);
				}
				playHitSound(_Player->getPos());
				deleteObject(_Bullets, i);
				i--;
				break;
			case GameObjectType::BulletObjectBoss:
				if (_GameMode != GameMode::Invincible)
				{
					_Player->takeDamage(5);
				}
				playHitSound(_Player->getPos());
				deleteObject(_Bullets, i);
				i--;
				break;
			}
		}
	}
	// Collision player with gameobjects
	for (unsigned int i = 0; i < _GameObjects.size(); i++) {
		_GameObjects[i]->update(FrameTime, RoadSpeed);
		if (_PlayerAlive && _Player->checkForCollision(*_GameObjects.at(i))) {
			switch (_GameObjects[i]->getType())
			{
			case GameObjectType::Canister:
				_Player->addEnergy();
				deleteObject(_GameObjects, i);
				i--;
				break;
			case GameObjectType::Tools:
				_Player->addHealth();
				deleteObject(_GameObjects, i);
				i--;
				break;
			}
		}
	}

	// Delete cars that go off screen
	for (unsigned int i = 0; i < _Cars.size(); i++)
	{
		if (_Cars.at(i)->getPos().y - _Cars.at(i)->getHeight() / 2 > SCREENHEIGHT || _Cars.at(i)->getPos().y + _Cars.at(i)->getHeight() / 2 <= 0 || _Cars.at(i)->getPos().x + _Cars.at(i)->getWidth() / 2 <= 0 || _Cars.at(i)->getPos().x - _Cars.at(i)->getWidth() / 2 >= SCREENWIDTH)
		{
			deleteObject(_Cars, i);
			i--;
		}
	}
	// Delete bullets that go off screen
	for (unsigned int i = 0; i < _Bullets.size(); i++) {
		if (_Bullets.at(i)->getPos().y - _Bullets.at(i)->getHeight() / 2 > SCREENHEIGHT || _Bullets.at(i)->getPos().y + _Bullets.at(i)->getHeight() / 2 <= 0 || _Bullets.at(i)->getPos().x + _Bullets.at(i)->getWidth() / 2 <= 0 || _Bullets.at(i)->getPos().x - _Bullets.at(i)->getWidth() / 2 >= SCREENWIDTH)
		{
			deleteObject(_Bullets, i);
			i--;
		}
	}
	// Delete gameobjects that go off screen
	for (unsigned int i = 0; i < _GameObjects.size(); i++) {
		if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getHeight() / 2 > SCREENHEIGHT || _GameObjects.at(i)->getPos().y + _GameObjects.at(i)->getHeight() / 2 <= 0 || _GameObjects.at(i)->getPos().x + _GameObjects.at(i)->getWidth() / 2 <= 0 || _GameObjects.at(i)->getPos().x - _GameObjects.at(i)->getWidth() / 2 >= SCREENWIDTH)
		{
			deleteObject(_GameObjects, i);
			i--;
		}
	}

	if (!_BossFight || (_BossFight && getBossCar()->getTraffic()))
	{
		if (!_AboutToLevelUp) {
			// Spawn AICar
			if (_TimePassedCar + FrameTime > 1 / _CarFrequency)
			{
				_TimePassedCar += FrameTime - 1 / _CarFrequency;
				spawnAICar(RoadSpeed);
			}
			else
			{
				_TimePassedCar += FrameTime;
			}
		}

		// Spawn bullet
		if (_TimePassedBullet + FrameTime > 1 / _BulletFrequency)
		{
			_TimePassedBullet += FrameTime - 1 / _BulletFrequency;
			//spawnBullet();

			setBulletFrequency();
		}
		else {
			_TimePassedBullet += FrameTime;
		}

		// Check AICars for collision
		for (unsigned int i = 0; i < _Cars.size(); i++)
		{
			for (unsigned int j = 0; j < _Cars.size(); j++)
			{
				if (i != j && _Cars.at(i)->getLane() == _Cars.at(j)->getLane() && _Cars.at(i)->getSpeed() != _Cars.at(j)->getSpeed())
				{
					if (std::abs(_Cars.at(i)->getPos().y - _Cars.at(j)->getPos().y) < _Cars.at(i)->getHeight() + 20)
					{
						int minSpeed = std::min({_Cars.at(i)->getSpeed(),_Cars.at(j)->getSpeed() });
						_Cars.at(i)->setSpeed(minSpeed);
						_Cars.at(j)->setSpeed(minSpeed);
					}
				}
			}
			for (unsigned int j = 0; j < _Bullets.size(); j++) {
				if (_Bullets[j]->getType() != GameObjectType::BulletObjectAI && _Cars.at(i)->checkForCollision(*_Bullets[j])) {
					if (_Bullets[j]->getType() == GameObjectType::BulletObjectPlayer) {
						_Cars.at(i)->takeDamage(_Player->getBulletdamage());
						playHitSound(_Bullets[j]->getPos());
					} else {
						_Cars.at(i)->takeDamage(500);
					}
					deleteObject(_Bullets, j);
					break;
				}
			}
		}

		// Delete destroyed cars
		for (unsigned int i = 0; i < _Cars.size(); i++)
		{
			if (_Cars.at(i)->getHealth() == 0)
			{
				_CarScore += (int)(1.5 * _Cars.at(i)->getMaxHealth());
				std::shared_ptr<Explosion> newExplosion(new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume));
				_Animations.push_back(newExplosion);
				deleteObject(_Cars, i);
				i--;
			}
		}
	}

	if (_BossFight)
	{
		_Boss->update(FrameTime, RoadSpeed, _Bullets, *_Player);

		// Collision player with boss
		if (_Player->checkForCollision(*_Boss)) {
			_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(_Player->getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
			_PlayerAlive = false;
		}
		for (unsigned int i = 0; i < _Bullets.size(); i++)
		{
			if (_Boss->checkForCollision(*_Bullets.at(i)) && _Bullets.at(i)->getType() == GameObjectType::BulletObjectPlayer)
			{
				_Boss->takeDamage(_Player->getBulletdamage());
				playHitSound(_Boss->getPos());
				deleteObject(_Bullets, i);
				i--;
			}
		}
		for (unsigned int i = 0; i < _Cars.size(); i++)
		{
			if (_Boss->checkForCollision(*_Cars[i]))
			{
				std::shared_ptr<Explosion> newExplosion(new Explosion(_Cars.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _Cars[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume));
				_Animations.push_back(newExplosion);
				deleteObject(_Cars, i);
				i--;
			}
		}
	}

	if (!_AboutToLevelUp) {
		// Spawn energy canister
		_TimePassedCanister += FrameTime;
		if (_TimePassedCanister > 1.0f / _CanisterFrequency)
		{
			_TimePassedCanister -= 1.0f / _CanisterFrequency;
			_GameObjects.push_back(GameObjectFactory::getCanister(sf::Vector2f(std::rand() % 3 * 150 + 150, -25)));
		}

		// Spawn toolbox
		_TimePassedToolbox += FrameTime;
		if (_TimePassedToolbox > 1.0f / _ToolboxFrequency && _GameMode != GameMode::Invincible)
		{
			_TimePassedToolbox -= 1.0f / _ToolboxFrequency;
			_GameObjects.push_back(GameObjectFactory::getToolbox(sf::Vector2f(std::rand() % 3 * 150 + 150, -10)));
			setToolboxFrequency();
		}

		if (_GameMode != GameMode::InfEnergy) {
			_Player->drainEnergy(FrameTime);
		}
	}

	// Check whether player fired a shot
	sf::Vector2f bulletDir = _Player->getShotBullet();
	if ((bulletDir.x != 0 || bulletDir.y != 0) && (_GameMode == GameMode::InfEnergy || _Player->drainShotEnergy())) {
		_Bullets.push_back(GameObjectFactory::getBullet(_Player->getPos(), bulletDir, _PlayerBulletSpeed, GameObjectType::BulletObjectPlayer, _SoundEffects, _Volume));
	}
}

void GameObjectContainer::render(sf::RenderWindow& window, bool renderCrosshair)
{
	for (unsigned int i = 0; i < _Cars.size(); i++)
	{
		_Cars.at(i)->render(window);
	}

	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		_GameObjects.at(i)->render(window);
	}
	for (unsigned int i = 0; i <_Bullets.size(); i++)
	{
		_Bullets.at(i)->render(window);
	}

	if (_BossFight) {
		_Boss->render(window);
	}

	if (_PlayerAlive) {
		_Player->render(window, renderCrosshair);
	}

	for (unsigned int i = 0; i < _Animations.size(); i++) {
		_Animations[i]->render(window);
	}
}

void GameObjectContainer::handleEvent(sf::Event& newEvent)
{
	if (_PlayerAlive) {
		_Player->handleEvent(newEvent);
		_Player->applyKeyPress(_Player->getPressedKeys());
	}
}

void GameObjectContainer::playSounds()
{
	for (unsigned int i = 0; i < _SoundEffects.size(); i++) {
		if (_SoundEffects[i].first->getStatus() == sf::Sound::Stopped || _SoundEffects[i].first->getStatus() == sf::Sound::Paused) {
			if (_SoundEffects[i].second) {
				_SoundEffects.erase(_SoundEffects.begin() + i);
			}
			else {
				_SoundEffects[i].first->play();
				_SoundEffects[i].second = true;
			}
		}
	}
}

void GameObjectContainer::stopSounds() 
{
	_SoundEffects.clear();
	if (_BossFight) {
		getBossCar()->stopSounds();
	}
}

bool GameObjectContainer::bossIsDead()
{
	if (_BossFight) {
		if (getBossCar()->getHealth() <= 0) {
			_AboutToLevelUp = true;
		}
		if (_AboutToLevelUp && getBossCar()->isDoneExploding(_ExplosionTexture)) {
			_BossFight = false;
			_AboutToLevelUp = false;
			_Player->resetResources();

			while (_GameObjects.size() != 0) {
				deleteObject(_GameObjects, 0);
			}
			while (_Cars.size() != 0) {
				deleteObject(_Cars, 0);
			}
			while (_Bullets.size() != 0) {
				deleteObject(_Bullets, 0);
			}

			_CarScore = 5000 + 10000 * _Difficulty * _Difficulty;

			return true;
		}
	}
	return false; 
}

void GameObjectContainer::enterBossFight()   
{
	_Boss = GameObjectFactory::getBossCar((_Level - 1) % 4, _Difficulty, getBossHP(), _SoundEffects, _ExplosionSoundBuffer, _Volume);
	_BossFight = true;
}

void GameObjectContainer::resetGameObjects(int SelectedCar)
{
	_GameObjects.clear();
	_Cars.clear();
	_Bullets.clear();
	_Animations.clear();
	_SoundEffects.clear();

	//Level
	_Level = 1;

	//Spielerauto
	_Player = GameObjectFactory::getPlayerCar(SelectedCar);
	
	//Frequenz
	setAllFrequencies();

	_TimePassedCar = 0.0f;
	_TimePassedBullet = 0.0f;
	_TimePassedCanister = 0.0f;
	_TimePassedToolbox = 0.0f;

	_PlayerAlive = true;
	_AboutToLevelUp = false;
	_BossFight = false;
}

bool GameObjectContainer::emptyScreen()
{
	_AboutToLevelUp = true;
	if (_Cars.size() == 0) {
		_AboutToLevelUp = false;
		return true;
	}
	return false;
}

void GameObjectContainer::load()
{
	GameObjectFactory::load();

	_ExplosionSoundBuffer.loadFromFile("Resources/Sound/explosion.wav");
	_ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");
	_ImpactSoundBuffer.loadFromFile("Resources/Sound/impact.wav");
}

void GameObjectContainer::setCarSkins(std::vector<std::shared_ptr<sf::Texture>>& CarSkins)
{
	GameObjectFactory::setPlayerCarTextures(CarSkins);
}

// TODO decide what is going to happen with this method
/*
	void GameObjectContainer::playShotSound(GameObjectType go, sf::Vector2f position)
	{
		std::shared_ptr<sf::Sound> shotSound(new sf::Sound());
		if (go == GameObjectType::AI) {
			shotSound = _GOFactory.getAIShotSound(position, _Volume);
		}
		else if (go == GameObjectType::Player) {
			shotSound->setBuffer(_PlayerShotSoundBuffer);
			shotSound->setVolume(_Volume * 2);
		}
	
		_SoundEffects.push_back({ shotSound, 0 });
	}
*/

void GameObjectContainer::spawnAICar(int roadSpeed)
{
	std::shared_ptr<AICar> newAiCar = GameObjectFactory::getAICar(getAiHP(), roadSpeed);

	for (unsigned int i = 1; i < _Cars.size(); i++)
	{
		if (_Cars.at(i)->getLane() == newAiCar->getLane() && _Cars.at(i)->getPos().y < _Cars.at(i)->getHeight() / 2 + 20)
		{
			return;
		}
	}

	_Cars.push_back(newAiCar);
}

void GameObjectContainer::spawnBullet()
{
	if (_Cars.size() == 0)
		return;

	std::shared_ptr<GameObject> SelectedCar = _Cars.at(std::rand() % _Cars.size());
	
	sf::Vector2f dir = SelectedCar->divideByLength(_Player->getPos() - SelectedCar->getPos());

	_Bullets.push_back(GameObjectFactory::getBullet(SelectedCar->getPos(), dir, _AIBulletSpeed, GameObjectType::BulletObjectAI, _SoundEffects, _Volume));
}

bool GameObjectContainer::playerIsAlive() {
	if (_Player->getEnergy() <= 0) {
		return false;
	}
	if (!_PlayerAlive && _Animations.size() > 0) {
		if (_Animations[_Animations.size() - 1]->getCurrentFrameNum() >= 18) {
			return false;
		}
	}
	return true;
}

void GameObjectContainer::setAllFrequencies()
{
	setAiCarFrequency();
	setBulletFrequency();
	setCanisterFrequency();
	setToolboxFrequency();
}

void GameObjectContainer::setAiCarFrequency()
{
	switch (_Difficulty)
	{
	case 0:
		_CarFrequency = 1.5f + 0.1f * (float)_Level;
		break;
	case 1:
		_CarFrequency = 1.75f + 0.11f * std::pow((float)_Level, 1.3f);
		break;
	case 2:
		_CarFrequency = 2.0f + 0.15f * std::pow((float)_Level, 1.3f);
		break;
	case 3:
		_CarFrequency = 2.15f + 0.17 * std::pow((float)_Level, 1.45f);
		break;
	default:
		break;
	}
}

void GameObjectContainer::setBulletFrequency()
{
	switch (_Difficulty)
	{
	case 0:
		_BulletFrequency = 0.8f + 0.065f * (float)_Level;
		break;
	case 1:
		_BulletFrequency = 1.2f + 0.08f * std::pow((float)_Level, 1.1f);
		break;
	case 2:
		_BulletFrequency = 1.2f + 1.0f * std::pow((float)_Level, 1.2f);
		break;
	case 3:
		_BulletFrequency = 1.4f + 1.0f * std::pow((float)_Level, 1.33f);
		break;
	default:
		break;
	}
}


void GameObjectContainer::setCanisterFrequency()
{
	switch (_Difficulty)
	{
	case 0:
		_CanisterFrequency = 0.5f;
		break;
	case 1:
		_CanisterFrequency = 0.4f;
		break;
	case 2:
		_CanisterFrequency = 0.3f;
		break;
	case 3:
		_CanisterFrequency = 0.3f;
		break;
	default:
		break;
	}
}


void GameObjectContainer::setToolboxFrequency()
{
	switch (_Difficulty)
	{
	case 0:
		_ToolboxFrequency = (float)(std::rand() % 45) / 1000.f + 0.080f;
		break;
	case 1:
		_ToolboxFrequency = (float)(std::rand() % 20) / 1000.f + 0.060f;
		break;
	case 2:
		_ToolboxFrequency = (float)(std::rand() % 20) / 1000.f + 0.040f;
		break;
	case 3:
		_ToolboxFrequency = (float)(std::rand() % 20) / 1000.f + 0.020f;
		break;
	default:
		break;
	}
}

int GameObjectContainer::getAiHP()
{
	switch (_Difficulty)
	{
	case 0:
		return 40 + _Level * 10;
		break;
	case 1:
		return 50 + _Level * 15;
		break;
	case 2:
		return 60 + _Level * 20;
		break;
	case 3:
		return 65 + _Level * 25;
		break;
	default:
		break;
	}
}

int GameObjectContainer::getBossHP()
{
	switch ((_Level - 1) % 4)
	{
	case 0:
		return 4500 + (int)((_Level - 1) / 4.0f) * 2500;
		break;
	case 1:
		return 5500 + (int)((_Level - 1) / 4.0f) * 2500;
		break;
	case 2:
		return 1500 + 750 * (int)((_Level - 1) / 4.0f);
		break;
	case 3:
		return 6500 + (int)((_Level - 1) / 4.0f) * 2500;
		break;
	}

}

void GameObjectContainer::playHitSound(sf::Vector2f position)
{
	std::shared_ptr<sf::Sound> ImpactSound = std::make_shared<sf::Sound>();
	ImpactSound->setBuffer(_ImpactSoundBuffer);
	ImpactSound->setVolume(_Volume * 5.5f);
	ImpactSound->setPosition(sf::Vector3f(position.x, 0, position.y));
	ImpactSound->setMinDistance(650.0f);
	ImpactSound->setAttenuation(2.0f);
	_SoundEffects.push_back(std::make_pair(ImpactSound, false));
}

