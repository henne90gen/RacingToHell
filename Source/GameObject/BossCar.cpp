#include "stdafx.h"
#include "GameObject/BossCar.h"

BossCar::BossCar(std::vector<sf::Texture*>& textures, sf::Vector2f Position) : Car(Position, 1000, 500, GameObjects::Boss, (*textures.at(0))),
	_GunOrientation(0.0f), _Phase(3), _Event1Frequency(0.0f), _Event2Frequency(0.0f), _BulletSpeed(500), _Event1Switch(false), _Event2Switch(false), _Event1Counter(0), _Event2Counter(0)
{
	_CannonSprite.setTexture(*textures.at(1));
	_CannonSprite.setOrigin(_CannonSprite.getLocalBounds().width / 2.0f, 50.0f);

	_BulletTexture = textures.at(2);

	_BossEventTimer1.restart();
	_BossEventTimer2.restart();
}

BossCar::~BossCar()
{
}

void BossCar::render(sf::RenderWindow& Window) {
	Window.draw(getSprite());
	Window.draw(_CannonSprite);
}

void BossCar::update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects)
{
	switch (_Phase)
	{
	case 0: //Auf Spieler schie�en
		_Event1Frequency = 2.0f;

		aimAtPlayer(GameObjects[0]);

		if (getBossEvent() == 1)
		{
			ShootBullet(GameObjects, getPos(), _GunOrientation);
		}
		break;
	case 1: //Spin
		_Event1Frequency = 5.0f;

		if (_GunOrientation + 50.0f * FrameTime > 360.0f)
		{
			_GunOrientation = _GunOrientation - 360.0f + 50.0f * FrameTime;
		}
		else
		{
			_GunOrientation += 30.0f * FrameTime;
		}

		if (getBossEvent() == 1)
		{
			ShootBullet(GameObjects, getPos(), _GunOrientation);
		}
		break;
	case 2: //3er Salve
		_Event1Frequency = 3.0f;
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
			if (getBossEvent() == 2)
			{
				_Event1Switch = true;
			}
		}
		break;
	case 3:
		_Event1Frequency = 0.5;

		if (getBossEvent() == 1)
		{
			for (int i = 0; i <= 360; i += 10)
			{
				sf::Vector2f Position = getPos() + sf::Vector2f(100.0f * std::sinf(i), 100.0f * std::cos(i));
				ShootBullet(GameObjects, Position, (float)i);
			}
		}
		break;
	default:
		break;
	}

	_CannonSprite.setPosition(getPos());
	_CannonSprite.setRotation(_GunOrientation - 90.0f);
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
	if (Player->getPos().x < getPos().x)
	{
		_GunOrientation = std::atanf((getPos().y - Player->getPos().y) / (getPos().x - Player->getPos().x)) * 180.0f / PI + 180;
	}
	else if (Player->getPos().x > getPos().x)
	{
		_GunOrientation = std::atanf((Player->getPos().y - getPos().y) / (Player->getPos().x - getPos().x)) * 180.0f / PI;
	}
	else
	{
		if (Player->getPos().y > getPos().y)
		{
			_GunOrientation = 90;
		}
		else
		{
			_GunOrientation = -90;
		}
	}
}

void BossCar::ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction)
{
	Bullet* newBullet = new Bullet(getPos(), Direction, _BulletSpeed, GameObjects::BulletObjectAI, *_BulletTexture);
	GameObjects.push_back(newBullet);
}
