#pragma once
#include "GameObject\Car.h"
#include "GameObject\Bullet.h"

class BossCar : public Car
{
public:
	BossCar(std::vector<sf::Texture*>& textures, sf::Vector2f Position);
	~BossCar();

	void render(sf::RenderWindow& Window);
	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);
private:
	sf::Texture* _BulletTexture;
	sf::Sprite _CannonSprite;

	sf::Clock _BossEventTimer1;
	sf::Clock _BossEventTimer2;
	float _Event1Frequency, _Event2Frequency;
	bool _Event1Switch, _Event2Switch;
	int _Event1Counter, _Event2Counter;

	int _Phase, _BulletSpeed;
	float _GunOrientation;

	int getBossEvent();
	void aimAtPlayer(GameObject* Player);
	void ShootBullet(std::vector<GameObject*>& GameObjects, sf::Vector2f Position, float Direction);
};

