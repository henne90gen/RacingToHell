#include "stdafx.h"
#include "GameObject\GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(400) {}

GameObjectContainer::~GameObjectContainer()
{
	_GameObjects.clear();
	_AICarTextures.clear();
	_BossCarTextures.clear();
}

void GameObjectContainer::update(float FrameTime, int RoadSpeed)
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
							std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume));
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
							std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume));
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
					std::shared_ptr<Explosion> newExplosion(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume));
					_Animations.push_back(newExplosion);
					_PlayerAlive = false;
				}
			}

			_GameObjects.at(i)->update(FrameTime, RoadSpeed);
		}
	}

	//Objekt löschen wenn es sich nicht mehr im Screen befindet
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
				spawnAICar(RoadSpeed);
			}
			else
			{
				_TimePassedCar += FrameTime;
			}
		}

		//Kaputte Autos löschen
		for (unsigned int i = 0; i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjectType::AI && _GameObjects.at(i)->getHealth() <= 0)
			{
				_CarScore += (int)(1.5 * _GameObjects.at(i)->getMaxHealth());
				std::shared_ptr<Explosion> newExplosion(new Explosion(_GameObjects.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _GameObjects[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume));
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

			setBulletFrequency();
		}
		else {
			_TimePassedBullet += FrameTime;
		}

		//AI-Autos auf Kollision prüfen
		for (unsigned int i = 1 + (int)(_BossFight); i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjectType::AI)
			{
				for (unsigned int j = 1 + (int)(_BossFight); j < _GameObjects.size(); j++)
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
					std::shared_ptr<Explosion> newExplosion(new Explosion(_GameObjects.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _GameObjects[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume));
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
		if (_TimePassedToolbox + FrameTime > 1.0f / _ToolboxFrequency)
		{
			_TimePassedToolbox += FrameTime - 1.0f / _ToolboxFrequency;
			std::shared_ptr<GameObject> newToolbox(new GameObject(sf::Vector2f(std::rand() % 3 * 150 + 150, -10), GameObjectType::Tools, _ToolboxTexture));
			_GameObjects.push_back(newToolbox);
			setToolboxFrequency();
		}
		else {
			_TimePassedToolbox += FrameTime;
		}

		getPlayerCar().drainEnergy(FrameTime);
	}

	//Prüfen ob Spieler geschossen hat
	if (getPlayerCar().shotBullet().x != 0 && getPlayerCar().shotBullet().y != 0)
	{
		std::shared_ptr<Bullet> newBullet(new Bullet(getPlayerCar().getPos(), getPlayerCar().shotBullet(), _PlayerBulletSpeed, GameObjectType::BulletObjectPlayer, _BulletTexture, _SoundEffects, _PlayerShotSoundBuffer, _Volume));

		_GameObjects.push_back(newBullet);

		getPlayerCar().resetShotBullet();
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
			getPlayerCar().resetResources();

			for (unsigned int i = 1; i < _GameObjects.size(); i++)
			{
				deleteObject(i);
			}

			_CarScore = 5000 + 10000 * _Difficulty * _Difficulty;

			return true;
		}
	}
	return false; 
}

void GameObjectContainer::enterBossFight()   
{
	switch ((_Level - 1) % 4)
	{
	case 0:
	{
		std::shared_ptr<Tank> boss(new Tank(_Difficulty, getBossHP(), (*_BossCarTextures[0]), _BulletTexture, _SoundEffects, _AIShotSoundBuffer, _ExplosionSoundBuffer, _Volume));
		_GameObjects.push_back(boss);
		break;
	}
		
	case 1:
	{
		std::shared_ptr<Mech> boss(new Mech(_Difficulty, getBossHP(), (*_BossCarTextures[2]), (*_BossCarTextures[3]), _BulletTexture, _SoundEffects, _AIShotSoundBuffer, _ExplosionSoundBuffer, _Volume));
		_GameObjects.push_back(boss);
		break;
	}
	case 2:
	{
		std::shared_ptr<Jet> boss(new Jet(_Difficulty, getBossHP(), (*_BossCarTextures[4]), _BulletTexture, _SoundEffects, _AIShotSoundBuffer, _ExplosionSoundBuffer, _JetSoundBuffer, _Volume));
		_GameObjects.push_back(boss);
		break;
	}	
	case 3:
	{
		std::shared_ptr<Carrier> boss(new Carrier(_Difficulty, getBossHP(), (*_BossCarTextures[1]), _BulletTexture, _SoundEffects, _AIShotSoundBuffer, _ExplosionSoundBuffer, _Volume));
	_GameObjects.push_back(boss);
		break;
	}
	}

	_BossFight = true;
	
}

void GameObjectContainer::resetGameObjects(int SelectedCar)
{
	_GameObjects.clear();
	_Animations.clear();
	_SoundEffects.clear();

	//Level
	_Level = 1;

	//Spielerauto
	std::shared_ptr<PlayerCar> MainCar(new PlayerCar(SelectedCar, (*_PlayerCarTextures.at(SelectedCar))));
	_GameObjects.push_back(MainCar);
	
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
	_ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");

	_AIShotSoundBuffer.loadFromFile("Resources/Sound/shotAI.wav");
	_PlayerShotSoundBuffer.loadFromFile("Resources/Sound/shotPlayer.wav");
	_ExplosionSoundBuffer.loadFromFile("Resources/Sound/explosion.wav");
	_JetSoundBuffer.loadFromFile("Resources/Sound/jetengine.wav");

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

void GameObjectContainer::playShotSound(GameObjectType go, sf::Vector2f position)
{
	std::shared_ptr<sf::Sound> shotSound(new sf::Sound());
	if (go == GameObjectType::AI) {
		shotSound->setBuffer(_AIShotSoundBuffer);
		shotSound->setPosition(position.x, 0.f, position.y);
		shotSound->setMinDistance(500.f);
		shotSound->setAttenuation(4.f); 
		shotSound->setVolume(_Volume * 2);
	}
	else if (go == GameObjectType::Player) {
		shotSound->setBuffer(_PlayerShotSoundBuffer);
		shotSound->setVolume(_Volume * 2);
	}
	
	_SoundEffects.push_back({ shotSound, 0 });
}


void GameObjectContainer::spawnAICar(int roadSpeed)
{
	std::shared_ptr<AICar> newAiCar(new AICar(getAiHP(), roadSpeed, (*_AICarTextures.at(std::rand() % 7))));

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

	std::shared_ptr<Bullet> newBullet(new Bullet(SelectedCar->getPos(), dir, _AIBulletSpeed, GameObjectType::BulletObjectAI, _BulletTexture, _SoundEffects, _AIShotSoundBuffer, _Volume));
	_GameObjects.push_back(newBullet);
	
	playShotSound(GameObjectType::AI, SelectedCar->getPos());
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
		_CarFrequency = 1.75f + 0.11f * std::powf((float)_Level, 1.3f);
		break;
	case 2:
		_CarFrequency = 2.0f + 0.15f * std::powf((float)_Level, 1.3f);
		break;
	case 3:
		_CarFrequency = 2.15f + 0.17 * std::powf((float)_Level, 1.45f);
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
		_BulletFrequency = 1.2f + 0.08f * std::powf((float)_Level, 1.1f);
		break;
	case 2:
		_BulletFrequency = 1.2f + 1.0f * std::powf((float)_Level, 1.2f);
		break;
	case 3:
		_BulletFrequency = 1.4f + 1.0f * std::powf((float)_Level, 1.33f);
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
		//return 0;
		break;
	case 1:
		return 5500 + (int)((_Level - 1) / 4.0f) * 2500;
		break;
	case 2:
		return 1000 + 500 * (int)((_Level - 1) / 4.0f);
		break;
	case 3:
		return 6500 + (int)((_Level - 1) / 4.0f) * 2500;
		break;
	}

}

