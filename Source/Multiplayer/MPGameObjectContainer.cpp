#include "stdafx.h"
#include "Multiplayer/MPGameObjectContainer.h"

MPGameObjectContainer::MPGameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(400) {}

MPGameObjectContainer::~MPGameObjectContainer()
{
	_GameObjects.clear();
}

void MPGameObjectContainer::update(float FrameTime, int RoadSpeed, NetworkHandle& network)
{
	if (getPlayer2Car().getType() != GameObjectType::Player) {
		// TODO: Add failsafe
	}

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



	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (playerIsAlive()) {
			if (i > 1) {
				if (getPlayerCar().checkForCollision(*_GameObjects.at(i))) {
					switch (_GameObjects[i]->getType())
					{
					case GameObjectType::AI:
					{
						//_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
						//_PlayerAlive = false;
						// TODO: send destruction of player
					}
					break;
					case GameObjectType::BulletObjectAI:
						getPlayerCar().takeDamage(5);
						playHitSound(getPlayerCar().getPos());
						// TODO: send player health
						deleteGameObject(i, false);
						i--;
						break;
					case GameObjectType::Canister:
						getPlayerCar().addEnergy();
						deleteGameObject(i, true);
						break;
					case GameObjectType::Tools:
						getPlayerCar().addHealth();
						deleteGameObject(i, true);
						break;
					case GameObjectType::Boss:
					{
						_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
						_PlayerAlive = false;
						// TODO: send destruction of player
					}
					break;
					case GameObjectType::BulletObjectBoss:
						getPlayerCar().takeDamage(5);
						playHitSound(getPlayerCar().getPos());
						// TODO: send player health
						deleteGameObject(i, false);
						i--;
						break;
					}
				}
			}
			else {
				if (getPlayerCar().getHealth() <= 0) {
					_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(getPlayerCar().getPos(), _ExplosionTexture, sf::Vector2f(0, 0), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
					_PlayerAlive = false;
					// TODO: send destruction of player
				}
			}
			_GameObjects.at(i)->update(FrameTime, RoadSpeed);
		}
	}

	// Check whether player fired a shot
	if (getPlayerCar().shotBullet().x != 0 && getPlayerCar().shotBullet().y != 0)
	{
		addGameObject(GameObjectFactory::getBullet(getPlayerCar().getPos(), getPlayerCar().shotBullet(), _PlayerBulletSpeed, GameObjectType::BulletObjectPlayer, _SoundEffects, _Volume), network);
		getPlayerCar().resetShotBullet();
	}

	//getPlayerCar().drainEnergy(FrameTime);
	if (_Relation == NetworkRelation::Host) {
		// Spawn energy canister
		if (_TimePassedCanister + FrameTime > 1 / _CanisterFrequency)
		{
			_TimePassedCanister += FrameTime - 1 / _CanisterFrequency;
			addGameObject(GameObjectFactory::getCanister(sf::Vector2f(std::rand() % 3 * 150 + 150, -25)), network);
		}
		else {
			_TimePassedCanister += FrameTime;
		}

		// Spawn toolbox
		if (_TimePassedToolbox + FrameTime > 1.0f / _ToolboxFrequency)
		{
			_TimePassedToolbox += FrameTime - 1.0f / _ToolboxFrequency;
			addGameObject(GameObjectFactory::getToolbox(sf::Vector2f(std::rand() % 3 * 150 + 150, -10)), network);
			setToolboxFrequency();
		}
		else {
			_TimePassedToolbox += FrameTime;
		}

		// Spawn AICar
		if (_TimePassedCar + FrameTime > 1 / _CarFrequency)
		{
			_TimePassedCar += FrameTime - 1 / _CarFrequency;
			spawnAICar(RoadSpeed, network);
		}
		else
		{
			_TimePassedCar += FrameTime;
		}
	}

	for (unsigned int i = 2 + (int)(_BossFight); i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjectType::AI)
		{
			for (unsigned int j = 2 + (int)(_BossFight); j < _GameObjects.size(); j++)
			{
				if (_GameObjects.at(j)->getType() == GameObjectType::AI && i != j &&
					_GameObjects.at(i)->getLane() == _GameObjects.at(j)->getLane() &&
					_GameObjects.at(i)->getSpeed() != _GameObjects.at(j)->getSpeed())
				{
					if (std::abs(_GameObjects.at(i)->getPos().y - _GameObjects.at(j)->getPos().y) < _GameObjects.at(i)->getHeight() + 20)
					{
						int minSpeed = std::min({ _GameObjects.at(i)->getSpeed(),_GameObjects.at(j)->getSpeed() });
						_GameObjects.at(i)->setSpeed(minSpeed);
						_GameObjects.at(j)->setSpeed(minSpeed);
					}
				}
				else if (_GameObjects.at(j)->getType() == GameObjectType::BulletObjectPlayer)
				{
					if (_GameObjects.at(i)->checkForCollision(*_GameObjects.at(j)))
					{
						_GameObjects.at(i)->takeDamage(getPlayerCar().getBulletdamage());
						playHitSound(_GameObjects.at(j)->getPos());
						if (_Relation == NetworkRelation::Host) {
							_SendObjects.push_back(std::make_pair(NetworkCommunication::UpdateAICar, _GameObjects.at(i)));
						}
						deleteGameObject(j, false);
						break;
					}
				}
				/*else if (_GameObjects.at(j)->getType() == GameObjectType::BulletObjectBoss)
				{
					if (_GameObjects.at(i)->checkForCollision(*_GameObjects.at(j)))
					{
						_GameObjects.at(i)->takeDamage(500);
						deleteGameObject(j);
						break;
					}
				}*/
			}
		}
	}

	// Delete destroyed cars
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjectType::AI && _GameObjects.at(i)->getHealth() == 0)
		{
			_CarScore += (int)(1.5 * _GameObjects.at(i)->getMaxHealth());
			_Animations.push_back(std::shared_ptr<Explosion>(new Explosion(_GameObjects.at(i)->getPos(), _ExplosionTexture, sf::Vector2f(0, _GameObjects[i]->getSpeed()), _SoundEffects, _ExplosionSoundBuffer, _Volume)));
			deleteGameObject(i, false);
			i--;
		}
	}
}

void MPGameObjectContainer::render(sf::RenderWindow& Window, bool renderCrosshair)
{
	for (unsigned int i = _GameObjects.size(); i > 0; i--)
	{
		if (_GameObjects.at(i - 1)->getType() == GameObjectType::Player && _PlayerAlive) {
			_GameObjects.at(i - 1)->render(Window, renderCrosshair);
		}
		else {
			_GameObjects.at(i - 1)->render(Window);
		}
	}

	for (int i = 0; i < _Animations.size(); i++) {
		_Animations[i]->render(Window);
	}
}

void MPGameObjectContainer::handleEvent(sf::Event& newEvent)
{
	if (_PlayerAlive) {
		getPlayerCar().handleEvent(newEvent);
	}
}

void MPGameObjectContainer::playSounds()
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

void MPGameObjectContainer::stopSounds()
{
	_SoundEffects.clear();
	if (_BossFight) {
		getBossCar().stopSounds();
	}
}

bool MPGameObjectContainer::bossIsDead()
{
	if (_BossFight) {
		if (getBossCar().getHealth() <= 0) {
			_AboutToLevelUp = true;
		}
		if (_AboutToLevelUp && getBossCar().isDoneExploding(_ExplosionTexture)) {
			_BossFight = false;
			_AboutToLevelUp = false;
			deleteGameObject(1, false);
			getPlayerCar().resetResources();

			while (_GameObjects.size() > 1)
			{
				deleteGameObject(1, false);
			}

			_CarScore = 5000 + 10000 * _Difficulty * _Difficulty;

			return true;
		}
	}
	return false;
}

void MPGameObjectContainer::enterBossFight()
{
	_GameObjects.push_back(GameObjectFactory::getBossCar((_Level - 1) % 4, _Difficulty, getBossHP(), _SoundEffects, _ExplosionSoundBuffer, _Volume));
	_BossFight = true;
}

void MPGameObjectContainer::resetGameObjects(int SelectedCar)
{
	_GameObjects.clear();
	_Animations.clear();
	_SoundEffects.clear();

	//Level
	_Level = 1;

	//Spielerauto
	_GameObjects.push_back(GameObjectFactory::getPlayerCar(SelectedCar));

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

bool MPGameObjectContainer::emptyScreen()
{
	_AboutToLevelUp = true;
	if (_GameObjects.size() == 1) {
		_AboutToLevelUp = false;
		return true;
	}
	return false;
}

void MPGameObjectContainer::load()
{
	GameObjectFactory::load();

	_ExplosionSoundBuffer.loadFromFile("Resources/Sound/explosion.wav");
	_ExplosionTexture.loadFromFile("Resources/Texture/Animation/explosion.png");
	_ImpactSoundBuffer.loadFromFile("Resources/Sound/impact.wav");
}

void MPGameObjectContainer::setCarSkins(std::vector<std::shared_ptr<sf::Texture>>& CarSkins)
{
	GameObjectFactory::setPlayerCarTextures(CarSkins);
}

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

void MPGameObjectContainer::spawnAICar(int roadSpeed, NetworkHandle& network)
{
	std::shared_ptr<AICar> newAiCar = GameObjectFactory::getAICar(getAiHP(), roadSpeed);

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

	addGameObject(newAiCar, network);
}

void MPGameObjectContainer::spawnBullet()
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

	_GameObjects.push_back(GameObjectFactory::getBullet(SelectedCar->getPos(), dir, _AIBulletSpeed, GameObjectType::BulletObjectAI, _SoundEffects, _Volume));
}

bool MPGameObjectContainer::playerIsAlive() {
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

void MPGameObjectContainer::setAllFrequencies()
{
	setAiCarFrequency();
	setBulletFrequency();
	setCanisterFrequency();
	setToolboxFrequency();
}

void MPGameObjectContainer::setAiCarFrequency()
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

void MPGameObjectContainer::setBulletFrequency()
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


void MPGameObjectContainer::setCanisterFrequency()
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


void MPGameObjectContainer::setToolboxFrequency()
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

int MPGameObjectContainer::getAiHP()
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

int MPGameObjectContainer::getBossHP()
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

void MPGameObjectContainer::playHitSound(sf::Vector2f position)
{
	std::shared_ptr<sf::Sound> ImpactSound = std::make_shared<sf::Sound>();
	ImpactSound->setBuffer(_ImpactSoundBuffer);
	ImpactSound->setVolume(_Volume * 5.5f);
	ImpactSound->setPosition(sf::Vector3f(position.x, 0, position.y));
	ImpactSound->setMinDistance(650.0f);
	ImpactSound->setAttenuation(2.0f);
	_SoundEffects.push_back(std::make_pair(ImpactSound, false));
}

void MPGameObjectContainer::addGameObject(std::shared_ptr<GameObject> newGO, NetworkHandle& network) {
	if (newGO->getType() == GameObjectType::AI) {
		std::lock_guard<std::mutex> lock(_Mutex);
		sf::Packet tmp;
		tmp << sf::Uint8(NetworkCommunication::CreateGameObject) << network.getTick();
		*newGO >> tmp;
		network.getReceivedPackets().push_back(tmp);
	}
	else {
		_GameObjects.push_back(newGO);
	}
	_SendObjects.push_back(std::make_pair(NetworkCommunication::CreateGameObject, newGO));
}

void MPGameObjectContainer::deleteGameObject(unsigned int id, bool sendDeletion)
{
	if (sendDeletion) {
		_SendObjects.push_back(std::make_pair(NetworkCommunication::DeleteGameObject, _GameObjects.at(id)));
	}
	else {
		_GameObjects.erase(_GameObjects.begin() + id);
	}
}

void MPGameObjectContainer::handleIncomingPackets(NetworkHandle& network) {
	std::lock_guard<std::mutex> lock(_Mutex);
	for (unsigned int i = 0; i < network.getReceivedPackets().size(); i++) {
		sf::Packet tmp = network.getReceivedPackets().at(i);
		sf::Uint8 recType;
		sf::Uint32 recTick;
		tmp >> recType >> recTick;

		switch ((NetworkCommunication)recType) {
		case NetworkCommunication::CreateGameObject:
			if (network.getTick() > recTick + network.getDelay()) {
				GameObjectFactory::scanPacketForGO(_Level, tmp, _GameObjects, _SoundEffects, _ExplosionSoundBuffer, _Volume);
				std::shared_ptr<GameObject> go = _GameObjects.at(_GameObjects.size() - 1);
				/*if (go->getType() == GameObjectType::BulletObjectPlayer) {
					int elapsedTicks = network.getTick() - recTick;
					float elapsedTime = (float)elapsedTicks / (float)network.getTickRate();
					go->setPos((float)go->getSpeed() * go->getDir() * elapsedTime);
				}*/
				network.getReceivedPackets().erase(network.getReceivedPackets().begin() + i);
				i--;
			}
			break;
		case NetworkCommunication::DeleteGameObject:
			sf::Uint32 id;
			tmp >> id;
			if (network.getTick() > recTick + network.getDelay()) {
				for (unsigned int j = 0; j < _GameObjects.size(); j++) {
					if (_GameObjects.at(j)->getID() == id) {
						deleteGameObject(j, false);
						break;
					}
				}
				network.getReceivedPackets().erase(network.getReceivedPackets().begin() + i);
				i--;
			}
			break;
		case NetworkCommunication::UpdateAICar:
		{
			GameObject go = GameObject(tmp, GameObjectType::AI);
			if (network.getTick() > recTick + network.getDelay()) {
				network.getReceivedPackets().at(i) >> recType >> recTick;
				for (unsigned int j = 0; j < _GameObjects.size(); j++) {
					if (_GameObjects.at(j)->getID() == go.getID()) {
						*_GameObjects.at(j) << network.getReceivedPackets().at(i);
						break;
					}
				}
				network.getReceivedPackets().erase(network.getReceivedPackets().begin() + i);
				i--;
			}
			break;
		}
		case NetworkCommunication::UpdateP2:
			sf::Uint8 type;
			tmp >> type;
			if (network.getTick() > recTick + network.getDelay()) {
				if ((GameObjectType)type == GameObjectType::Player) {
					/*
					sf::Uint32 id;
					float x, y;
					tmp >> id >> x >> y;
					std::cout << "Receiving ID: " << (unsigned int)id << " || X: " << x << " || Y: " << y << std::endl;
					*/
					*_GameObjects[1] << tmp;
				}
				network.getReceivedPackets().erase(network.getReceivedPackets().begin() + i);
				i--;
			}
			break;
		default:
			network.getReceivedPackets().erase(network.getReceivedPackets().begin() + i);
			i--;
			break;
		}
	}
}

void MPGameObjectContainer::handleOutgoingPackets(std::vector<std::pair<NetworkCommunication, sf::Packet>>& packets)
{
	if (_SendTimer.getElapsedTime().asSeconds() > 1.0f / 15.0f)
	{
		std::lock_guard<std::mutex> lock(_Mutex);
		sf::Packet tmp;
		getPlayerCar() >> tmp;
		packets.push_back(std::make_pair(NetworkCommunication::UpdateP2, tmp));

		_SendTimer.restart();
	}

	while (_SendObjects.size() > 0) {
		std::lock_guard<std::mutex> lock(_Mutex);
		sf::Packet tmp;
		if (_SendObjects.at(0).first == NetworkCommunication::DeleteGameObject) {
			tmp << _SendObjects.at(0).second->getID();
			for (unsigned int i = 0; i < _GameObjects.size(); i++) {
				if (_GameObjects.at(i)->getID() == _SendObjects.at(0).second->getID()) {
					deleteGameObject(i, false);
					break;
				}
			}
		}
		else {
			*_SendObjects.at(0).second >> tmp;
		}
		packets.push_back(std::make_pair(_SendObjects.at(0).first, tmp));
		_SendObjects.erase(_SendObjects.begin());
	}
}

