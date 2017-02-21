#include "stdafx.h"
#include "GameObject/AICar.h"


AICar::AICar(unsigned int id, int hp, int roadSpeed, sf::Texture &texture, sf::IntRect textureRect) :
		Car(id, sf::Vector2f(0, 0), hp, std::rand() % (roadSpeed / 3) + roadSpeed / 3,
			GameObjectType::AI, texture, textureRect) {
    _Lane = (sf::Uint8) (std::rand() % 4);
    init();
    getSprite().setColor(sf::Color::White);
}

AICar::AICar(sf::Packet &packet, sf::Texture &texture, sf::IntRect textureRect) :
        Car(packet, GameObjectType::AI, texture, textureRect) {
    AICar::operator<<(packet);
    init();
}

void AICar::update(float frameTime, int roadSpeed) {
    Car::update(frameTime, roadSpeed);

	float margin = 0.5f * frameTime * (float)_Speed;
	sf::Vector2f newPos;

	if (getPos().x >= laneToX() - margin && getPos().x <= laneToX() + margin)
	{
		newPos.x = laneToX();
	}
	else if (getPos().x < laneToX() - margin)
	{
		newPos.x = getPos().x + _Speed * frameTime;
	}
	else
	{
		newPos.x = getPos().x - _Speed * frameTime;
	}

	newPos.y = getPos().y + (roadSpeed - _Speed) * frameTime;

    setPos(newPos);

    _HealthBar.setPosition(sf::Vector2f(getPos().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2,
                                        getPos().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
    _HealthBar.setSize(
            sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
    _HealthBarFrame.setPosition(_HealthBar.getPosition());
}

void AICar::render(sf::RenderWindow &window) {
    window.draw(getSprite());
    window.draw(_HealthBar);
    window.draw(_HealthBarFrame);
}

void AICar::operator>>(sf::Packet &packet) {
    Car::operator>>(packet);
    write(packet, _Lane);
}

void AICar::operator<<(sf::Packet &packet) {
    Car::operator<<(packet);
    read(packet, _Lane);
}

inline int AICar::laneToX()
{
	return _Lane * SCREENWIDTH / 4 + SCREENWIDTH / 8;
}

void AICar::init() {
    // offset is needed, otherwise car is deleted immediately
    int offset = 1;
    setPos(sf::Vector2f(laneToX(), -1 * getHeight() / 2 + offset));

    _HealthBar.setFillColor(sf::Color(200, 0, 0));
    _HealthBar.setSize(sf::Vector2f(45, 5));

    _HealthBarFrame.setFillColor(sf::Color::Transparent);
    _HealthBarFrame.setOutlineColor(sf::Color(20, 0, 0));
    _HealthBarFrame.setOutlineThickness(1);
    _HealthBarFrame.setSize(_HealthBar.getSize());
}

void AICar::switchLaneRandomly()
{
	if (_Lane == 0)
	{
		_Lane = 1;
	}
	else if (_Lane == 3)
	{
		_Lane = 2;
	}
	else
	{
		int rnd = rand() % 2;

		if (rnd == 0)
			_Lane -= 1;
		else
			_Lane += 1;
	}
}