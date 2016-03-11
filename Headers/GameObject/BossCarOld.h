
#pragma once
#include "GameObject\Car.h"
#include "GameObject\Bullet.h"
#include "GameObject\PlayerCar.h"
#include "GameObject\AICar.h"

/*
class BossCarOld : public Car
{
public:
	BossCaOld(std::vector<sf::Texture*>& textures, sf::Vector2f Position, bool Traffic);
	~BossCarOld();

	void render(sf::RenderWindow& Window);
	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);

	bool getTrafficOn() { return _Traffic; }
private:
	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarFrame;

	sf::Texture* _BulletTexture;
	sf::Sprite _CannonSprite;

	sf::Clock _PhaseClock;
	float _ChangePhaseFrequency;

	sf::Clock _BossEventTimer1;
	sf::Clock _BossEventTimer2;
	float _Event1Frequency, _Event2Frequency;
	bool _Event1Switch, _Event2Switch;
	int _Event1Counter, _Event2Counter;

	int _Phase, _BulletSpeed;
	float _GunOrientation;

	sf::Vector2f _NextPosition, _DefaultPosition;
	float _Speed;
	int _MovementBehaviour;
	bool _Attack, _MovementSwitch;

	bool _Traffic;

	int getBossEvent();
	void aimAtPlayer(GameObject* Player);
	void ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction);
	bool DriveToNextPosition(float FrameTime);
};

*/