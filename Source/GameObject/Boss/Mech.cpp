#include "stdafx.h"
#include "GameObject/Boss/Mech.h"

Mech::Mech(unsigned int id, int difficulty, int HP, sf::Texture& textureTop, sf::Texture& textureLegs, sf::Texture& bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer &soundBufferShot, sf::SoundBuffer &soundBufferExplosion, float volume) :
	BossCar(id, sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), difficulty, HP, 100, textureTop, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, volume),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false)
{
	init();
}

Mech::Mech(sf::Packet & packet, sf::Texture & textureTop, sf::Texture & textureLegs, sf::Texture & bulletTexture, std::vector<std::pair<std::shared_ptr<sf::Sound>, bool>>& soundEffects, sf::SoundBuffer & soundBufferShot, sf::SoundBuffer & soundBufferExplosion, float volume) :
	BossCar(packet, textureTop, bulletTexture, soundEffects, soundBufferShot, soundBufferExplosion, volume),
	_TopAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureTop), _LegsAnim(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT + 100), textureLegs), _MovementSwitch(false) 
{
	init();
}

void Mech::render(sf::RenderWindow & window)
{
	_LegsAnim.render(window);
	_TopAnim.render(window);

	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);

	renderExplosions(window);
}

void Mech::update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>>& bullets, PlayerCar& player)
{
	Car::update(frameTime, roadSpeed);

	float gunPosLength = std::sqrt(std::pow(_GunPosition.x, 2) + std::pow(_GunPosition.y, 2));
	_GunPosition = sf::Vector2f(cosf(std::atan(_GunOrientation.y / _GunOrientation.x) + PI / 2),
								sinf(std::atan(_GunOrientation.y / _GunOrientation.x) + PI / 2)) * gunPosLength;

	if (!_IsExploding) {
		if (driveToNextPosition(frameTime))
		{
			switch (_Movement)
			{
			case Movement::DRIVETODEFAULT:
				_Movement = Movement::LEFTRIGHT;
				_Attack = true;
				_PhaseClock.restart();
				_Speed = _BaseSpeed;
				break;
			case Movement::LEFTRIGHT:
				_MovementSwitch = !_MovementSwitch;

				if (_MovementSwitch)
				{
					_NextPosition = getPos() + sf::Vector2f((SCREENWIDTH - getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				else
				{
					_NextPosition = getPos() - sf::Vector2f((getPos().x - getWidth() / 2) * (std::rand() % 100) / 100.0f, 0.0f);
				}
				break;
			case Movement::RUNATPLAYER:
				_Movement = Movement::DRIVETODEFAULT;
				_NextPosition = _DefaultPosition;
				break;
			case Movement::ZICKZACK:
				if (_Event1Counter < 4) {
					_Event1Counter++;
					_NextPosition = sf::Vector2f(std::abs((int)_Event2Switch * SCREENWIDTH - 65) + std::pow(-1, (int)_Event2Switch) * (_Event1Counter % 2) * (SCREENWIDTH - 130), (SCREENHEIGHT / 5) * _Event1Counter);
				}
				else {
					_Event1Counter = 0;
					_Movement = Movement::DRIVETODEFAULT;
				}
			default:
				break;
			}
		}

		if (_Attack)
		{
			switch (_Pattern[_CurrentPhase].first)
			{
			case Phase::SPIN:
			{
				_Event1Frequency = 11.0f + 3.0f * (float)_Difficulty;

				float gunAngle = getAngleFromVector(_GunOrientation) + (180 + 135 * _Difficulty) * frameTime;
				_GunOrientation = sf::Vector2f(std::cos(gunAngle * PI / 180), std::sin(gunAngle * PI / 180));

				if (getBossEvent() == 1)
				{
					std::pair<sf::Vector2f, sf::Vector2f> Positions = calcGunPositions();

					BossCar::shootBullet(bullets, Positions.first, _GunOrientation);
					BossCar::shootBullet(bullets, Positions.second, _GunOrientation);
				}
				break;
			}
			case Phase::SHOTGUN:
				//_GunOrientation = divideByLength(bullets[0]->getPos() - getPos());

				_Event1Frequency = 1.0f + 0.25f * (float)(_Difficulty);

				if (getBossEvent() == 1)
				{			
					bool Hand = (std::rand() % 100) > 50;
					for (int i = 0; i < 8 + 2 * _Difficulty; i++)
					{
						float bulAngle = (getAngleFromVector(_GunOrientation) * PI / 180.f - PI / 28.8f) + PI / 14.4f * ((std::rand() % 100) / 100.0f);
						sf::Vector2f bulOrientation = divideByLength(sf::Vector2f(std::cos(bulAngle), std::sin(bulAngle)));
						if (Hand)
						{
							shootBullet(bullets, calcGunPositions().first, bulOrientation, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed), (float)(i % 3 > 0) * _Volume);
						}
						else
						{
							shootBullet(bullets, calcGunPositions().second, bulOrientation, 0.75 * _BulletSpeed + ((std::rand() % 100) / 100.0f * 0.5 * _BulletSpeed), (float)(i % 3 > 0) * _Volume);
						}
					}

					_Event1Counter = 0;
					_Event1Switch = !_Event1Switch;
				}
				break;
			case Phase::SALVE:
				//_GunOrientation = divideByLength(bullets[0]->getPos() - getPos());

				_Event1Frequency = 1.25f + 0.25f * (float)_Difficulty;

				if (getBossEvent() == 1)
				{
					bool Hand = _Event1Switch;//(std::rand() % 100) > 50;
					_Event1Switch = !_Event1Switch;

					float bulAngle = getAngleFromVector(_GunOrientation);
					
					int NumberofBullets = (int)(5 + 0.5f * (float)_Difficulty);
					float dAngle = (42.5f + 2.5f * (float)_Difficulty) / (float)NumberofBullets;

					for (int i = 0; i < NumberofBullets; i++)
					{
						sf::Vector2f bulOrientation = sf::Vector2f(std::cos((bulAngle + (i - (int)(NumberofBullets / 2)) * dAngle) / 180 * PI), std::sin((bulAngle + (i - (int)(NumberofBullets / 2)) * dAngle) / 180 * PI));

						if (Hand)
						{
							BossCar::shootBullet(bullets, calcGunPositions().first, bulOrientation);
						}
						else
						{
							BossCar::shootBullet(bullets, calcGunPositions().second, bulOrientation);
						}
					}
				}
				break;
			case Phase::SALVEZICKZACK:
				//_GunOrientation = divideByLength(player.getPos() - getPos());

				_Event1Frequency = 1.25f + 0.25f * (float)_Difficulty;

				if (getBossEvent() == 1)
				{
					bool Hand = _Event1Switch;//(std::rand() % 100) > 50;
					_Event1Switch = !_Event1Switch;

					float bulAngle = getAngleFromVector(_GunOrientation);

					int NumberofBullets = (int)(3 + 0.5f * (float)_Difficulty);
					float dAngle = (37.0f + 2.5f * (float)_Difficulty) / (float)NumberofBullets;

					for (int i = 0; i < NumberofBullets; i++)
					{
						sf::Vector2f bulOrientation = sf::Vector2f(std::cos((bulAngle + (i - (int)(NumberofBullets / 2)) * dAngle) / 180 * PI), std::sin((bulAngle + (i - (int)(NumberofBullets / 2)) * dAngle) / 180 * PI));

						if (Hand)
						{
							BossCar::shootBullet(bullets, calcGunPositions().first, bulOrientation);
						}
						else
						{
							BossCar::shootBullet(bullets, calcGunPositions().second, bulOrientation);
						}
					}
				}
				break;
			case Phase::RUNATPLAYERPHASE:
				if (!_Event1Switch)
				{
					_NextPosition = player.getPos();
					_Movement = Movement::RUNATPLAYER;
					_Speed = (3.5 + 2 * _Difficulty) * _BaseSpeed;
					_Attack = false;

					_Event1Switch = true;
				}
				break;
			case Phase::ZICKZACKPHASE:
				if (!_Event1Switch) {
					_NextPosition = _DefaultPosition;
					_Movement = Movement::ZICKZACK;
					_CurrentPhase++;
					_Speed = (2 + _Difficulty) * _BaseSpeed;
					_Attack = true;
					_Event1Switch = true;
					_Event2Switch = (std::rand() % 100) > 50;
				}
			}
		}

		_TopAnim.setRotation(getAngleFromVector(_GunOrientation) + 90);
		_LegsAnim.setRotation(getAngleFromVector(_GunOrientation) + 90);

		updateHealthBar();

		if (_Movement != Movement::DRIVETODEFAULT && _Movement != Movement::ZICKZACK)
		{
			checkPhase();
		}

		_LegsAnim.update(frameTime);
		_TopAnim.update(frameTime);
	}
	else {
		updateExplosions(frameTime);
	}
}

void Mech::setPos(sf::Vector2f pos) {
	_TopAnim.getSprite().setPosition(pos);
	_LegsAnim.getSprite().setPosition(pos);
}

void Mech::init()
{
	setSprite(_TopAnim.getSprite());

	_GunOrientation = sf::Vector2f(0, -1);
	_GunPosition = sf::Vector2f(62, 0);
	_GunLength = 53.0f;

	//HP-Balken
	_HealthBar.setSize(sf::Vector2f(getWidth() + 5, 5));
	_HealthBarFrame.setSize(_HealthBar.getSize());

	_DefaultPosition = sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 200);
	_NextPosition = _DefaultPosition;
	_Movement = Movement::DRIVETODEFAULT;

	_BaseSpeed = _Speed;

	_Pattern = {
		std::make_pair(Phase::SPIN, ((2.0f + (float)_Difficulty) * 360.0f) / (180.0f + 135.f * (float)_Difficulty)) , std::make_pair(Phase::NOTHING, 1.5f),
		std::make_pair(Phase::SALVE, 7.0f), std::make_pair(Phase::NOTHING, 0.75f),
		std::make_pair(Phase::RUNATPLAYERPHASE, 0.25f),
		std::make_pair(Phase::SHOTGUN, 7.0f), std::make_pair(Phase::NOTHING, 1.5f),
		std::make_pair(Phase::ZICKZACKPHASE, 0.25f), std::make_pair(Phase::SALVEZICKZACK, 3.0f), std::make_pair(Phase::NOTHING, 0.75f)
	};
}

void Mech::shootBullet(std::vector<std::shared_ptr<Bullet>>& bullets, sf::Vector2f pos, sf::Vector2f dir, int bulletSpeed, float volume)
{
	bullets.push_back(GameObjectFactory::getBullet(pos, dir, bulletSpeed, GameObjectType::BulletObjectBoss, _soundEffects, volume));
}

std::pair<sf::Vector2f, sf::Vector2f> Mech::calcGunPositions()
{
	sf::Vector2f pos1 = getPos() - _GunPosition + _GunOrientation * _GunLength;
	sf::Vector2f pos2 = getPos() + _GunPosition + _GunOrientation * _GunLength;
	return std::make_pair(pos1, pos2);
}
