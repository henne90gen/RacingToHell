#include "stdafx.h"
#include "GameObject\GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(400) {}

GameObjectContainer::~GameObjectContainer()
{
	_GameObjects.clear();
	_AICarTextures.clear();
	_BossCarTextures.clear();
}

void GameObjectContainer::update(float FrameTime, int Difficulty, int RoadSpeed)
{
	// Update Animations
	for (int i = 0; i < _Animations.size(); i++) {
		if (_Animations[i]->getAnimationState() == Animation::Stop) {
			_Animations.erase(_Animations.begin() + i);
			i--;
		}
		else {
			_Animations[i]->update(FrameTime);
		}
	}

	//Kollision Spieler
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_PlayerAlive) {
			if (i > 0) {
				if (getPlayerCar().checkForCollision(*_GameObjects.at(i))) {
					switch (_GameObjects[i]->getType())
					{
					case GameObjectType::AI:
						{	
							std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0)));
							_Animations.push_back(newExplosion);
							_PlayerAlive = false;
						}
						break;
					case GameObjectType::BulletObjectAI:
						getPlayerCar().takeDamage(5);
						deleteObject(i);
						i--;
						break;
					case GameObjectType::Canister:
						getPlayerCar().addEnergy();
						deleteObject(i);
						i--;
						break;
					case GameObjectType::Tools:
						getPlayerCar().addHealth();
						deleteObject(i);
						i--;
						break;
					case GameObjectType::Boss:
						{
							std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0)));
							_Animations.push_back(newExplosion);
							_PlayerAlive = false;
						}
						break;
					case GameObjectType::BulletObjectBoss:
						getPlayerCar().takeDamage(5);
						deleteObject(i);
						i--;
						break;
					}
				}
			}
			else {
				if (getPlayerCar().getHealth() <= 0) {
					std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0)));
					_Animations.push_back(newExplosion);
					_PlayerAlive = false;
				}
			}

			_GameObjects.at(i)->update(FrameTime, RoadSpeed);
		}
	}

	//Objekt l�schen wenn es sich nicht mehr im Screen befindet
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() != GameObjectType::Boss)
		{
			if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getHeight() / 2 > SCREENHEIGHT || _GameObjects.at(i)->getPos().y + _GameObjects.at(i)->getHeight() / 2 <= 0 || _GameObjects.at(i)->getPos().x + _GameObjects.at(i)->getWidth() / 2 <= 0 || _GameObjects.at(i)->getPos().x - _GameObjects.at(i)->getWidth() / 2 >= SCREENWIDTH)
			{
				deleteObject(i);
				i--;
			}
		}
	}

	if (!_BossFight || (_BossFight && getBossCar().getTraffic())) 
	{
		if (!_AboutToLevelUp) {
			//AI-Autos spawnen
			if (_TimePassedCar + FrameTime > 1 / _CarFrequency)
			{
				_TimePassedCar += FrameTime - 1 / _CarFrequency;
				spawnAICar(Difficulty, RoadSpeed);
			}
			else
			{
				_TimePassedCar += FrameTime;
			}
		}

		//Kaputte Autos l�schen
		for (unsigned int i = 0; i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjectType::AI && _GameObjects.at(i)->getHealth() <= 0)
			{
				_CarScore += (int)(1.5 * _GameObjects.at(i)->getMaxHealth());
				std::shared_ptr<Explosion> newExplosion(new Explosion(_GameObjects.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _GameObjects[i]->getSpeed())));
				_Animations.push_back(newExplosion);
				deleteObject(i);
				i--;
			}
		}

		//Bullets spawnen
		if (_TimePassedBullet + FrameTime > 1 / _BulletFrequency)
		{
			_TimePassedBullet += FrameTime - 1 / _BulletFrequency;
			spawnBullet();

			_BulletFrequency = 1.0f + 0.1f * (float)(Difficulty);
		}
		else {
			_TimePassedBullet += FrameTime;
		}

		//AI-Autos auf Kollision pr�fen
		for (unsigned int i = 1 + (int)(_BossFight); i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjectType::AI)
			{
				for (unsigned int j = i + 1; j < _GameObjects.size(); j++)
				{
					if (_GameObjects.at(j)->getType() == GameObjectType::AI && i != j &&
						_GameObjects.at(i)->getLane() == _GameObjects.at(j)->getLane() &&
						_GameObjects.at(i)->getSpeed() != _GameObjects.at(j)->getSpeed())
					{
						if (std::abs(_GameObjects.at(i)->getPos().y - _GameObjects.at(j)->getPos().y) < _GameObjects.at(i)->getHeight() + 20)
						{
							int minSpeed = std::min({_GameObjects.at(i)->getSpeed(),_GameObjects.at(j)->getSpeed() });
							_GameObjects.at(i)->setSpeed(minSpeed);
							_GameObjects.at(j)->setSpeed(minSpeed);
						}
					}
					else if (_GameObjects.at(j)->getType() == GameObjectType::BulletObjectPlayer)
					{
						if (_GameObjects.at(i)->checkForCollision(*_GameObjects.at(j)))
						{
							_GameObjects.at(i)->takeDamage(getPlayerCar().getBulletdamage());
							deleteObject(j);
							break;
						}
					}
					else if (_GameObjects.at(j)->getType() == GameObjectType::BulletObjectBoss)
					{
						if (_GameObjects.at(i)->checkForCollision(*_GameObjects.at(j)))
						{
							_GameObjects.at(i)->takeDamage(500);
							deleteObject(j);
							break;
						}
					}
				}
			}
		}
	}
	if (_BossFight)
	{
		for (unsigned int i = 2; i < _GameObjects.size(); i++)
		{
			if (getBossCar().checkForCollision(*_GameObjects.at(i)))
			{
				if (_GameObjects.at(i)->getType() == GameObjectType::BulletObjectPlayer)
				{
					_GameObjects.at(1)->takeDamage(getPlayerCar().getBulletdamage());
					deleteObject(i);
					i--;
				}
				else if (_GameObjects.at(i)->getType() == GameObjectType::AI)
				{
					std::shared_ptr<Explosion> newExplosion(new Explosion(_GameObjects.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _GameObjects[i]->getSpeed())));
					_Animations.push_back(newExplosion);
			
					deleteObject(i);
					i--;
				}
			}
		}

		getBossCar().update(FrameTime, RoadSpeed, _GameObjects);
	} 

	if (!_AboutToLevelUp) {
		//EnergyCanister spawnen
		if (_TimePassedCanister + FrameTime > 1 / _CanisterFrequency)
		{
			_TimePassedCanister += FrameTime - 1 / _CanisterFrequency;
			std::shared_ptr<GameObject> newCanister(new GameObject(sf::Vector2f(std::rand() % 3 * 150 + 150, -25), GameObjectType::Canister, _EnergyCanisterTexture));
			_GameObjects.push_back(newCanister);
		}
		else {
			_TimePassedCanister += FrameTime;
		}

		//Toolbox spawnen
		if (_TimePassedToolbox + FrameTime > 1 / _ToolboxFrequency)
		{
			_TimePassedToolbox += FrameTime - 1 / _ToolboxFrequency;
			_ToolboxFrequency = (float)(std::rand() % 150) / 1000.0f;
			std::shared_ptr<GameObject> newToolbox(new GameObject(sf::Vector2f(std::rand() % 3 * 150 + 150, -10), GameObjectType::Tools, _ToolboxTexture));
			_GameObjects.push_back(newToolbox);
		}
		else {
			_TimePassedToolbox += FrameTime;
		}
	}

	//Pr�fen ob Spieler geschossen hat
	if (getPlayerCar().shotBullet().x != 0 && getPlayerCar().shotBullet().y != 0)
	{
		std::shared_ptr<Bullet> newBullet(new Bullet(getPlayerCar().getPos(), getPlayerCar().shotBullet(), _PlayerBulletSpeed, GameObjectType::BulletObjectPlayer, _BulletTexture));

		_GameObjects.push_back(newBullet);

		getPlayerCar().resetShotBullet();

		playShotSound(GameObjectType::Player);
	}
}

void GameObjectContainer::render(sf::RenderWindow& Window, bool renderCrosshair)
{
	for (unsigned int i = _GameObjects.size(); i > 0; i--)
	{
		if (_GameObjects.at(i-1)->getType() == GameObjectType::Player && _PlayerAlive) {
			_GameObjects.at(i-1)->render(Window, renderCrosshair);
		}
		else {
			_GameObjects.at(i-1)->render(Window);
		}
	}

	for (int i = 0; i < _Animations.size(); i++) {
		_Animations[i]->render(Window);
	}
}

void GameObjectContainer::handleEvent(sf::Event& newEvent)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_PlayerAlive) {
			_GameObjects.at(i)->handleEvent(newEvent);
		}
	}
}

void GameObjectContainer::playSounds()
{
	for (int i = 0; i < _SoundEffects.size(); i++) {
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

bool GameObjectContainer::bossIsDead()
{
	if (_BossFight) {
		if (getBossCar().getHealth() <= 0) {
			_AboutToLevelUp = true;
		}
		if (_AboutToLevelUp && getBossCar().isDoneExploding(_ExplosionTexture)) {
			_BossFight = false;
			_AboutToLevelUp = false;
			deleteObject(1);
			return true;
		}
	}
	return false; 
}

void GameObjectContainer::enterBossFight()   
{
	//Tank* boss = new Tank((*_BossCarTextures[0]), _BulletTexture);
	//Carrier* boss = new Carrier(_BossCarTextures[1], &_BulletTexture);
	std::shared_ptr<Mech> boss(new Mech((*_BossCarTextures[2]), (*_BossCarTextures[3]), _BulletTexture));
	//Jet* boss = new Jet((*_BossCarTextures[4]), _BulletTexture);
	_GameObjects.push_back(boss);
	_BossFight = true;
}

void GameObjectContainer::resetGameObjects(int SelectedCar)
{
	_GameObjects.clear();
	_Animations.clear();
	_SoundEffects.clear();

	//Spielerauto
	std::shared_ptr<PlayerCar> MainCar(new PlayerCar(SelectedCar, (*_PlayerCarTextures.at(SelectedCar))));
	_GameObjects.push_back(MainCar);

	//Frequenz
	_CarFrequency = 2;
	_BulletFrequency = 1.0f;
	_CanisterFrequency = 0.3f;
	_ToolboxFrequency = (float)(std::rand() % 150) / 1000.0f;

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
	if (_GameObjects.size() == 1) {
		_AboutToLevelUp = false;
		return true;
	}
	return false;
}

void GameObjectContainer::load()
{
	for (int i = 0; i < 7; i++) {
		std::shared_ptr<sf::Texture> texture(new sf::Texture());
		(*texture).loadFromFile("Resources/Texture/TrafficCar/Traffic" + std::to_string(i + 1) + ".png");
		_AICarTextures.push_back(texture);
	}

	_BulletTexture.loadFromFile("Resources/Texture/Object/Bullet.png");
	_ToolboxTexture.loadFromFile("Resources/Texture/Object/toolbox.png");
	_EnergyCanisterTexture.loadFromFile("Resources/Texture/Object/canister.png");

	_AIShotSoundBuffer.loadFromFile("Resources/Sound/shotAI.wav");
	_PlayerShotSoundBuffer.loadFromFile("Resources/Sound/shotPlayer.wav");
	
	_ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");

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
}

void GameObjectContainer::setCarSkins(std::vector<std::shared_ptr<sf::Texture>>& CarSkins)
{
	_PlayerCarTextures = CarSkins;
}

void GameObjectContainer::playShotSound(GameObjectType go)
{
	std::shared_ptr<sf::Sound> shotSound(new sf::Sound());
	if (go == GameObjectType::AI) {
		shotSound->setBuffer(_AIShotSoundBuffer);
	}
	else if (go == GameObjectType::Player) {
		shotSound->setBuffer(_PlayerShotSoundBuffer);
	}
	shotSound->setVolume(_Volume);
	_SoundEffects.push_back({ shotSound, 0 });
}

void GameObjectContainer::spawnAICar(int difficulty, int roadSpeed)
{
	std::shared_ptr<AICar> newAiCar(new AICar(difficulty, roadSpeed, (*_AICarTextures.at(std::rand() % 7))));

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjectType::AI)
		{
			if (_GameObjects.at(i)->getLane() == newAiCar->getLane() && _GameObjects.at(i)->getPos().y < _GameObjects.at(i)->getHeight() / 2 + 20)
			{
				return;
			}
		}
	}

	_GameObjects.push_back(newAiCar);
}

void GameObjectContainer::spawnBullet()
{
	std::vector<std::shared_ptr<GameObject>> AICarVector;

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjectType::AI)
		{
			AICarVector.push_back(_GameObjects.at(i));
		}
	}

	if (AICarVector.size() == 0)
		return;

	std::shared_ptr<GameObject> SelectedCar = AICarVector.at(std::rand() % AICarVector.size());

	sf::Vector2f dir = SelectedCar->divideByLength(getPlayerCar().getPos() - SelectedCar->getPos());

	std::shared_ptr<Bullet> newBullet(new Bullet(SelectedCar->getPos(), dir, _AIBulletSpeed, GameObjectType::BulletObjectAI, _BulletTexture));
	_GameObjects.push_back(newBullet);
	
	playShotSound(GameObjectType::AI);
}

bool GameObjectContainer::playerIsAlive() {
	if (getPlayerCar().getEnergy() <= 0) {
		return false;
	}
	if (!_PlayerAlive && _Animations.size() > 0) {
		if (_Animations[_Animations.size() - 1]->getCurrentFrameNum() >= 18) {
			return false;
		}
	}
	return true;
}

void GameObjectContainer::deleteObject(unsigned int id)
{
	_GameObjects.erase(_GameObjects.begin() + id);
}
