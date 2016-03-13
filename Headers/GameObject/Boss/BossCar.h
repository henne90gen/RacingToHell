#pragma once

#include "GameObject\Car.h"
#include "GameObject\GameObjects.h"
#include "GameObject\Bullet.h"

class BossCar : public Car
{
public:
	BossCar(sf::Vector2f& Position, int Health, float Speed, sf::Texture& Texture, sf::Texture& BulletTexture);
	~BossCar();

	virtual void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects) = 0;
	virtual void render(sf::RenderWindow& RenderWindow) = 0;

	bool getTraffic() { return _Traffic; }
protected:
	enum Phase { NOTHING, SIMPLESHOOT, SALVE, SPIN, HARDCORESPAM, BLASTSALVE, SPIRAL, RANDOMSPRAY, SHOTGUN, SIDE };
	int _CurrentPhase;
	std::vector<std::pair<Phase, float>> _Pattern;
	sf::Clock _PhaseClock;

	enum Movement { STILL, DRIVETODEFAULT, LEFTRIGHT, SWITCHSIDES, STRAIGHT, PARABOLA};
	Movement _Movement;

	sf::Vector2f _DefaultPosition;

	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarFrame;

	sf::Texture _BulletTexture;

	int const _BulletSpeed;

	sf::Clock _BossEventTimer1;
	sf::Clock _BossEventTimer2;
	float _Event1Frequency, _Event2Frequency;
	bool _Event1Switch, _Event2Switch;
	int _Event1Counter, _Event2Counter;

	int getBossEvent();

	sf::Vector2f _NextPosition;
	bool _Attack, _Traffic;

	float PlayerAngle(GameObject* Player);
	void ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction) { ShootBullet(GameObjects, Position, Direction, _BulletSpeed); }
	void ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction, int BulletSpeed);
	bool DriveToNextPosition(float FrameTime);
	void updateHealthBar();
	void checkPhase();
};