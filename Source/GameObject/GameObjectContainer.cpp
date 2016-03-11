#include "stdafx.h"
#include "GameObject\GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(400)
{
}

GameObjectContainer::~GameObjectContainer()
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		delete _GameObjects.at(i);
		_GameObjects.at(i) = nullptr;
	}

	_GameObjects.clear();

	for (int i = 0; i < _AICarTextures.size(); i++) {
		delete _AICarTextures.at(i);
		_AICarTextures.at(i) = nullptr;
	}

	_AICarTextures.clear();
}

void GameObjectContainer::update(float FrameTime, int Difficulty, int RoadSpeed)
{
	// Update Animations
	for (int i = 0; i < _Animations.size(); i++) {
		if (_Animations[i]->getAnimationState() == Animation::Stop) {
			delete _Animations.at(i);
			_Animations.at(i) = nullptr;
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
		if (i > 0) {
			if (getPlayerCar()->checkForCollision(_GameObjects.at(i))) {
				switch (_GameObjects.at(i)->getType())
				{
				case GameObjects::AI:
					_PlayerAlive = false;
					break;
				case GameObjects::BulletObjectAI:
					getPlayerCar()->takeDamage(5);
					deleteObject(i);
					i--;
					break;
				case GameObjects::Canister:
					getPlayerCar()->addEnergy();
					deleteObject(i);
					i--;
					break;
				case GameObjects::Tools:
					getPlayerCar()->addHealth();
					deleteObject(i);
					i--;
					break;
				case GameObjects::Boss:
					_PlayerAlive = false;
					break;
				}
			}
		}
		else {
			if (getPlayerCar()->getHealth() <= 0 || getPlayerCar()->getEnergy() <= 0) {
				_PlayerAlive = false;
			}
		}

		_GameObjects.at(i)->update(FrameTime, RoadSpeed);
	}

	//Objekt löschen wenn es sich nicht mehr im Screen befindet
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() != GameObjects::Boss)
		{
			if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getHeight() / 2 > SCREENHEIGHT || _GameObjects.at(i)->getPos().y + _GameObjects.at(i)->getHeight() / 2 <= 0 || _GameObjects.at(i)->getPos().x + _GameObjects.at(i)->getWidth() / 2 <= 0 || _GameObjects.at(i)->getPos().x - _GameObjects.at(i)->getWidth() / 2 >= SCREENWIDTH)
			{
				deleteObject(i);
				i--;
			}
		}
	}

	if (!_BossFight || (false)) 
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

		//Kaputte Autos löschen
		for (unsigned int i = 0; i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjects::AI && dynamic_cast<AICar*>(_GameObjects.at(i))->getHealth() <= 0)
			{
				_CarScore += (int)(1.5 * dynamic_cast<AICar*>(_GameObjects.at(i))->getMaxHealth());
				_Animations.push_back(new Explosion(sf::Vector2f(_GameObjects.at(i)->getPos())));
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

		//AI-Autos auf Kollision prüfen
		for (unsigned int i = 1 + (int)(_BossFight); i < _GameObjects.size(); i++)
		{
			if (_GameObjects.at(i)->getType() == GameObjects::AI)
			{
				for (unsigned int j = i + 1; j < _GameObjects.size(); j++)
				{
					if (_GameObjects.at(j)->getType() == GameObjects::AI && i != j &&
						dynamic_cast<AICar*>(_GameObjects.at(i))->getLane() == dynamic_cast<AICar*>(_GameObjects.at(j))->getLane() &&
						dynamic_cast<AICar*>(_GameObjects.at(i))->getSpeed() != dynamic_cast<AICar*>(_GameObjects.at(j))->getSpeed())
					{
						if (std::abs(_GameObjects.at(i)->getPos().y - _GameObjects.at(j)->getPos().y) < _GameObjects.at(i)->getHeight() + 20)
						{
							int minSpeed = std::min({ dynamic_cast<AICar*>(_GameObjects.at(i))->getSpeed(), dynamic_cast<AICar*>(_GameObjects.at(j))->getSpeed() });
							dynamic_cast<AICar*>(_GameObjects.at(i))->setSpeed(minSpeed);
							dynamic_cast<AICar*>(_GameObjects.at(j))->setSpeed(minSpeed);
						}
					}
					else if (_GameObjects.at(j)->getType() == GameObjects::BulletObjectPlayer)
					{
						if (_GameObjects.at(i)->getSprite().getGlobalBounds().intersects(_GameObjects.at(j)->getSprite().getGlobalBounds()))
						{
							dynamic_cast<AICar*>(_GameObjects.at(i))->takeDamage(getPlayerCar()->getBulletdamage());
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
			if (getBossCar()->checkForCollision(_GameObjects.at(i)))
			{
				if (_GameObjects.at(i)->getType() == GameObjects::BulletObjectPlayer)
				{
					dynamic_cast<BossCar*>(_GameObjects.at(1))->takeDamage(getPlayerCar()->getBulletdamage());
					deleteObject(i);
					i--;
				}
				else if (_GameObjects.at(i)->getType() == GameObjects::AI)
				{
					deleteObject(i);
					i--;
				}
			}
		}

		getBossCar()->update(FrameTime, RoadSpeed, _GameObjects);
	} 

	if (!_AboutToLevelUp) {
		//EnergyCanister spawnen
		if (_TimePassedCanister + FrameTime > 1 / _CanisterFrequency)
		{
			_TimePassedCanister += FrameTime - 1 / _CanisterFrequency;
			EnergyCanister* canister = new EnergyCanister(sf::Vector2f(std::rand() % 3 * 150 + 150, -25), _EnergyCanisterTexture);
			_GameObjects.push_back(canister);
		}
		else {
			_TimePassedCanister += FrameTime;
		}

		//Toolbox spawnen
		if (_TimePassedToolbox + FrameTime > 1 / _ToolboxFrequency)
		{
			_TimePassedToolbox += FrameTime - 1 / _ToolboxFrequency;
			_ToolboxFrequency = (float)(std::rand() % 150) / 1000.0f;
			Toolbox* toolbox = new Toolbox(sf::Vector2f(std::rand() % 3 * 150 + 150, -10), _ToolboxTexture);
			_GameObjects.push_back(toolbox);
		}
		else {
			_TimePassedToolbox += FrameTime;
		}
	}

	//Prüfen ob Spieler geschossen hat
	if (getPlayerCar()->shotBullet() != 360.0f)
	{
		Bullet* newBullet = new Bullet(getPlayerCar()->getPos(), getPlayerCar()->shotBullet(), _PlayerBulletSpeed, GameObjects::BulletObjectPlayer, _BulletTexture);
		_GameObjects.push_back(newBullet);

		getPlayerCar()->resetShotBullet();

		playShotSound(GameObjects::Player);
	}
}

void GameObjectContainer::render(sf::RenderWindow& Window, bool renderCrosshair)
{
	for (unsigned int i = _GameObjects.size(); i > 0; i--)
	{
		if (_GameObjects.at(i-1)->getType() == GameObjects::Player) {
			dynamic_cast<PlayerCar*>(_GameObjects.at(i-1))->render(Window, renderCrosshair);
		}
		else {
			_GameObjects.at(i-1)->render(Window);
		}
	}

	for (int i = 0; i < _Animations.size(); i++) {
		_Animations[i]->render(Window);
	}
}

void GameObjectContainer::handleEvents(sf::Event& Event)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		_GameObjects.at(i)->handleEvent(Event);
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
	if (_BossFight)
		if (dynamic_cast<BossCar*>(_GameObjects.at(1))->getHealth() <= 0)
			//TODO: Remove boss and resets
			return true;
	return false; 
}

void GameObjectContainer::enterBossFight(bool entering)
{
	//Tank* boss = new Tank(_BossCarTextures[0], &_BulletTexture);
	Carrier* boss = new Carrier(_BossCarTextures[1], &_BulletTexture);
	_GameObjects.push_back(boss);
	_BossFight = entering;
}

void GameObjectContainer::resetGameObjects(int SelectedCar)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		delete _GameObjects.at(i);
		_GameObjects.at(i) = nullptr;
	}
	_GameObjects.clear();

	//Spielerauto
	PlayerCar* MainCar = new PlayerCar(SelectedCar, (*_PlayerCarTextures.at(SelectedCar)));
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
		sf::Texture* texture = new sf::Texture();
		(*texture).loadFromFile("Resources/Texture/TrafficCar/Traffic" + std::to_string(i + 1) + ".png");
		_AICarTextures.push_back(texture);
	}

	_BulletTexture.loadFromFile("Resources/Texture/Object/Bullet.png");
	_ToolboxTexture.loadFromFile("Resources/Texture/Object/toolbox.png");
	_EnergyCanisterTexture.loadFromFile("Resources/Texture/Object/canister.png");

	_AIShotSoundBuffer.loadFromFile("Resources/Sound/shotAI.wav");
	_PlayerShotSoundBuffer.loadFromFile("Resources/Sound/shotPlayer.wav");
				   

	_BossCarTextures.resize(2);
	_BossCarTextures[0].loadFromFile("Resources/Texture/BossCar/Tank.png");
	_BossCarTextures[1].loadFromFile("Resources/Texture/BossCar/Carrier.png");
}

void GameObjectContainer::setCarSkins(std::vector<sf::Texture*>& CarSkins)
{
	_PlayerCarTextures = CarSkins;
}

void GameObjectContainer::playShotSound(GameObjects go)
{
	sf::Sound* shotSound = new sf::Sound();
	if (go == GameObjects::AI) {
		shotSound->setBuffer(_AIShotSoundBuffer);
	}
	else if (go == GameObjects::Player) {
		shotSound->setBuffer(_PlayerShotSoundBuffer);
	}
	shotSound->setVolume(_Volume);
	_SoundEffects.push_back({ shotSound, 0 });
}

void GameObjectContainer::spawnAICar(int Difficulty, int RoadSpeed)
{
	AICar* newAiCar = new AICar(Difficulty, RoadSpeed, (*_AICarTextures.at(std::rand() % 7)));

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			if (dynamic_cast<AICar*>(_GameObjects.at(i))->getLane() == newAiCar->getLane() && _GameObjects.at(i)->getPos().y < _GameObjects.at(i)->getHeight() / 2 + 20)
			{
				delete newAiCar;
				newAiCar = nullptr;
				return;
			}
		}
	}

	_GameObjects.push_back(newAiCar);
}

void GameObjectContainer::spawnBullet()
{
	std::vector<AICar*> AICarVector;

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			AICarVector.push_back(dynamic_cast<AICar*>(_GameObjects.at(i)));
		}
	}

	if (AICarVector.size() == 0)
		return;

	AICar* SelectedCar = AICarVector.at(std::rand() % AICarVector.size());
	
	float Direction;
	if (getPlayerCar()->getPos().x < SelectedCar->getPos().x)
	{
		Direction = std::atanf((SelectedCar->getPos().y - getPlayerCar()->getPos().y) / (SelectedCar->getPos().x - getPlayerCar()->getPos().x)) * 180.0f / PI + 180;
	}
	else if (getPlayerCar()->getPos().x > SelectedCar->getPos().x)
	{
		Direction = std::atanf((getPlayerCar()->getPos().y - SelectedCar->getPos().y) / (getPlayerCar()->getPos().x - SelectedCar->getPos().x)) * 180.0f / PI;
	}
	else
	{
		if (getPlayerCar()->getPos().y > SelectedCar->getPos().y)
		{
			Direction = 90;
		}
		else
		{
			Direction = -90;
		}
	}

	Bullet* newBullet = new Bullet(SelectedCar->getPos(), Direction, _AIBulletSpeed, GameObjects::BulletObjectAI, _BulletTexture);
	_GameObjects.push_back(newBullet);
	
	playShotSound(GameObjects::AI);
}

void GameObjectContainer::deleteObject(unsigned int id)
{
	delete _GameObjects.at(id);
	_GameObjects.at(id) = nullptr;
	_GameObjects.erase(_GameObjects.begin() + id);
}
