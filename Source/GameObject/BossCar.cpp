#include "stdafx.h"
#include "GameObject/BossCar.h"

BossCar::BossCar(std::vector<sf::Texture*>& textures, sf::Vector2f Position, bool Traffic) : Car(sf::Vector2f(SCREENWIDTH / 2, -1 * (int)textures[0]->getSize().y / 2 + 1), 2000, 500, GameObjects::Boss, (*textures.at(0))),
	_GunOrientation(0.0f), _Phase(0), _Event1Frequency(0.0f), _Event2Frequency(0.0f), _BulletSpeed(500), _Event1Switch(false), _Event2Switch(false), _Event1Counter(0), _Event2Counter(0),
	_Speed(200.0f), _MovementBehaviour(-1), _Attack(false), _MovementSwitch(false), _ChangePhaseFrequency(0.25f), _Traffic(Traffic)
{
	_CannonSprite.setTexture(*textures.at(1));
	_CannonSprite.setOrigin(_CannonSprite.getLocalBounds().width / 2.0f, 50.0f);

	_BulletTexture = textures.at(2);

	_DefaultPosition = Position;
	_NextPosition = _DefaultPosition;

	_BossEventTimer1.restart();
	_BossEventTimer2.restart();
	_PhaseClock.restart();

	//HP-Balken
	_HealthBar.setFillColor(sf::Color(200, 0, 0));
	_HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));

	_HealthBarFrame.setFillColor(sf::Color::Transparent);
	_HealthBarFrame.setOutlineColor(sf::Color(20, 0, 0));
	_HealthBarFrame.setOutlineThickness(1);
	_HealthBarFrame.setSize(_HealthBar.getSize());
}

BossCar::~BossCar()
{
}

void BossCar::render(sf::RenderWindow& Window) {
	Window.draw(getSprite());
	Window.draw(_CannonSprite);
	Window.draw(_HealthBar);
	Window.draw(_HealthBarFrame);
}

void BossCar::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	bool Arrived = DriveToNextPosition(FrameTime);

	if (Arrived)
	{
		switch (_MovementBehaviour) {
		case -1:
			_MovementBehaviour = 3;
			_Attack = true;
			break;
		case 0:
		{
			_MovementSwitch = !_MovementSwitch;
			_Speed = 200;

			if (_MovementSwitch)
			{
				_NextPosition = getPos() + sf::Vector2f((SCREENWIDTH - getPos().x) * (std::rand() % 100) / 100.0f, 0.0f);
			}
			else
			{
				_NextPosition = getPos() - sf::Vector2f(getPos().x * (std::rand() % 100) / 100.0f, 0.0f);
			}

			_Attack = true;

			int dice = std::rand() % 100;

			if (dice > 95)
			{
				_NextPosition = GameObjects[0]->getPos();
				_MovementBehaviour = 1;
				_Speed = 700.0f;
				_Attack = false;
			}
			else if (dice > 100)
			{
				_NextPosition = sf::Vector2f(GameObjects[0]->getPos().x, getPos().y);
				_MovementBehaviour = 2;
				_Speed = dynamic_cast<PlayerCar*>(GameObjects[0])->getSpeed();
			}
			break;
		}
		case 1:
		{
			_NextPosition = _DefaultPosition;
			_MovementBehaviour = 0;
			_Speed = 400.0f;
			break;
		}
		case 2:
		{
			_NextPosition = sf::Vector2f(GameObjects[0]->getPos().x, getPos().y);
			break;
		}
		case 3:
		{
			_NextPosition = sf::Vector2f(getWidth() / 2 + (SCREENWIDTH - getWidth()) * (std::rand() % 100) / 100, getHeight() / 2 + (SCREENHEIGHT - getHeight()) * (std::rand() % 100) / 100);
			_Speed = 450.0f;
		}
		default:
			break;
		}
	}

	switch (_MovementBehaviour)
	{
	case 0: //Links-Rechts
		
		break;
	default:
		break;
	}

	if (_Attack)
	{
		switch (_Phase)
		{
		case 0: //Auf Spieler schießen
			_Event1Frequency = 4.0f;

			aimAtPlayer(GameObjects[0]);

			if (getBossEvent() == 1)
			{
				ShootBullet(GameObjects, getPos(), _GunOrientation);
			}
			break;
		case 1: //Spin
			_Event1Frequency = 11.0f;

			if (_GunOrientation > 180.0f || _GunOrientation < 0.0f)
			{
				_GunOrientation = (int)(!_Event1Switch) * 180.0f;
				_Event1Switch = !_Event1Switch;
			}
			else
			{
				_GunOrientation += 100.0f * FrameTime * std::pow(-1, (int)_Event1Switch);
			}

			if (getBossEvent() == 1)
			{
				ShootBullet(GameObjects, getPos(), _GunOrientation);
			}
			break;
		case 2: //3er Salve
			_Event1Frequency = 1.0f;
			_Event2Frequency = 10.0f;

			aimAtPlayer(GameObjects[0]);

			if (_Event1Switch)
			{
				if (getBossEvent() == 2)
				{
					ShootBullet(GameObjects, getPos(), _GunOrientation);

					if (_Event1Counter + 1 < 3)
					{
						_Event1Counter += 1;
					}
					else
					{
						_Event1Switch = false;
						_Event1Counter = 0;
					}
				}
			}
			else
			{
				if (getBossEvent() == 1)
				{
					_Event1Switch = true;
				}
			}
			break;
		case 3: //Druckwelle
		{

			_Event1Frequency = 1.0f;

			aimAtPlayer(GameObjects[0]);

			int dice = (std::rand() % 2) * 5;

			if (getBossEvent() == 1)
			{
				for (int i = dice; i <= 360 + dice; i += 10)
				{
					//sf::Vector2f Position = getPos() + sf::Vector2f(100.0f * std::sinf(i), 100.0f * std::cosf(i));
					ShootBullet(GameObjects, getPos(), (float)i);
				}
			}
			break;
		}
		case 4: //Druckwelle Salve
			_Event1Frequency = 0.4f;
			_Event2Frequency = 7.0f;

			aimAtPlayer(GameObjects[0]);

			if (_Event1Switch)
			{
				if (getBossEvent() == 2)
				{
					for (int i = 2 * _Event1Counter; i <= 360; i += 20)
					{
						ShootBullet(GameObjects, getPos(), (float)i);
					}

					if (_Event1Counter + 1 < 5)
					{
						_Event1Counter += 1;
					}
					else
					{
						_Event1Switch = false;
						_Event1Counter = 0;
					}
				}
			}
			else
			{
				if (getBossEvent() == 1)
				{
					_Event1Switch = true;
				}
			}
			break;
		case 5: //5er Salve
			_Event1Frequency = 1.5f;

			aimAtPlayer(GameObjects[0]);

			if (getBossEvent() == 1)
			{
				for (int i = 0; i < 5; i++)
				{
					ShootBullet(GameObjects, getPos(), _GunOrientation - 10 * (i - 2));
				}
			}
			break;
		case 6: //Spriale
			_Event1Frequency = 3.0f;
			_Event2Frequency = 10.0f;

			_GunOrientation = 90;

			if (_Event1Switch)
			{
				if (getBossEvent() == 2)
				{
					for (float i = 0.0f; i < 360.0f; i += 36.0f)
					{
						sf::Vector2f Position = getPos() + sf::Vector2f(50.0f * std::cosf(i / 180 * PI), 50.0f * std::sinf(i / 180 * PI));
						float Orientation = 90 + i;

						ShootBullet(GameObjects, Position, Orientation);
					}
				}
			}
			else
			{
				if (getBossEvent() == 1)
				{
					_Event1Switch = true;
				}
			}
			break;
		case 7: //Random Spray
			_Event1Frequency = 0.8f;

			aimAtPlayer(GameObjects[0]);

			if (getBossEvent() == 1)
			{
				for (float i = -30.0f; i < 240.0f; i += 10.0f)
				{
					sf::Vector2f Position = getPos() + sf::Vector2f(50.0f * std::cosf(i / 180 * PI), 50.0f * std::sinf(i / 180 * PI));
					float Orientation = (std::rand() % 270) - 30.0f;

					ShootBullet(GameObjects, Position, Orientation);
				}
			}
			break;
		case 8: //Hardcore Random Spam
			_Event1Frequency = 40.0f;

			if (getBossEvent() == 1)
			{
				float Orientation = (std::rand() % 270) - 30.0f;
				_GunOrientation = Orientation;

				ShootBullet(GameObjects, getPos(), Orientation);
			}
			break;
		case 9: //von der Seite
			_Event1Frequency = 0.4f;

			_GunOrientation = 90;

			if (getBossEvent() == 1)
			{
				for (int i = -40; i <= SCREENHEIGHT; i += 200)
				{
					ShootBullet(GameObjects, sf::Vector2f(0, i), 0.0f);
					ShootBullet(GameObjects, sf::Vector2f(SCREENWIDTH, i + 100), 180.0f);
				}
			}
			break;
		case 10:

		default:
			break;
		}
	}

	if (_PhaseClock.getElapsedTime().asSeconds() > 1 / _ChangePhaseFrequency)
	{
		if (std::rand() % 100 > 30)
		{
			_Phase = std::rand() % 10;
			_PhaseClock.restart();
		}
	}
	
	_CannonSprite.setPosition(getPos());
	_CannonSprite.setRotation(_GunOrientation - 90.0f);

	_HealthBar.setPosition(sf::Vector2f(getPos().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2, getPos().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
	_HealthBar.setSize(sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
	_HealthBarFrame.setPosition(_HealthBar.getPosition());
}

int BossCar::getBossEvent()
{
	if (_Event1Frequency != 0 && _BossEventTimer1.getElapsedTime().asSeconds() > 1 / _Event1Frequency)
	{
		_BossEventTimer1.restart();
		return 1;
	}
	else if (_Event2Frequency != 0 && _BossEventTimer2.getElapsedTime().asSeconds() > 1 / _Event2Frequency)
	{
		_BossEventTimer2.restart();
		return 2;
	}
	else
	{
		return 0;
	}
}

void BossCar::aimAtPlayer(GameObject* Player)
{
	sf::Vector2f dir = Player->getPos() - getPos();
	_GunOrientation = std::atan(dir.y / dir.x) * 180.0f / PI;
	if (dir.x < 0)
		_GunOrientation += 180.0f;
}

void BossCar::ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction)
{
	Bullet* newBullet = new Bullet(Position, Direction, _BulletSpeed, GameObjects::BulletObjectAI, *_BulletTexture);
	GameObjects.push_back(newBullet);
}

bool BossCar::DriveToNextPosition(float FrameTime)
{
	if (std::abs((getPos().y - _NextPosition.y)) < 0.1f && std::abs((getPos().x - _NextPosition.x)) < 0.1f)
	{
		return true;
	}
	else {
		sf::Vector2f movement = sf::Vector2f(_NextPosition.x - getPos().x, _NextPosition.y - getPos().y);
		float length = std::sqrtf(std::pow(movement.x, 2) + std::pow(movement.y, 2));
		movement = movement / length;

		setPos(getPos() + movement * FrameTime * _Speed);
		return false;
	}
}
