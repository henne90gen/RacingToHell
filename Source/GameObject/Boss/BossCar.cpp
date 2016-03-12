#include "stdafx.h"
#include "GameObject/Boss/BossCar.h"

BossCar::BossCar(sf::Vector2f& Position, int Health, float Speed, sf::Texture& Texture, sf::Texture* BulletTetxure) : Car(Position, Health, Speed, GameObjects::Boss, Texture),
	_BulletSpeed(500), _BulletTexture(BulletTetxure), _Movement(Movement::STILL), _Attack(false),
	_Event1Counter(0), _Event2Counter(0), _Event1Frequency(0), _Event2Frequency(0), _Event1Switch(false), _Event2Switch(false), _CurrentPhase(0)
{
	_BossEventTimer1.restart();
	_BossEventTimer2.restart();
	_PhaseClock.restart();

	_Pattern = { std::make_pair(Phase::NOTHING, 1.0f) };

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

float BossCar::PlayerAngle(GameObject * Player)
{
	float Angle;
	sf::Vector2f dir = Player->getPos() - getPos();

	Angle = std::atan(dir.y / dir.x) * 180.0f / PI;
	if (dir.x < 0)
		Angle += 180.0f;

	return Angle;
}

void BossCar::ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction)
{
	Bullet* newBullet = new Bullet(Position, Direction, _BulletSpeed, GameObjects::BulletObjectAI, *_BulletTexture);
	GameObjects.push_back(newBullet);
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

		setPos(getPos() + movement * FrameTime * (float)_Speed);
		return false;
	}
}

void BossCar::updateHealthBar()
{
	_HealthBar.setPosition(sf::Vector2f(getPos().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2, getPos().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
	_HealthBar.setSize(sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
	_HealthBarFrame.setPosition(_HealthBar.getPosition());
}

void BossCar::checkPhase()
{
	if (_PhaseClock.getElapsedTime().asSeconds() > _Pattern[_CurrentPhase].second)
	{
		if (_CurrentPhase + 1 >= _Pattern.size())
		{
			_CurrentPhase = 0;
		}
		else
		{
			_CurrentPhase++;
		}

		_PhaseClock.restart();
	}
}
