#include "stdafx.h"
#include "GameObject/AICar.h"


AICar::AICar(int hp, int roadSpeed, sf::Texture& texture) : Car(sf::Vector2f(0, 0), hp, std::rand() % ((int)(roadSpeed / 3)) + ((int)(roadSpeed / 3)), GameObjectType::AI, texture)
{
	_Lane = std::rand() % 4;
	setPos(sf::Vector2f(_Lane * SCREENWIDTH / 4 + SCREENWIDTH / 8, -getHeight() /2));

	//HP-Balken
	_HealthBar.setFillColor(sf::Color(200, 0, 0));
	_HealthBar.setSize(sf::Vector2f(45, 5));

	_HealthBarFrame.setFillColor(sf::Color::Transparent);
	_HealthBarFrame.setOutlineColor(sf::Color(20, 0, 0));
	_HealthBarFrame.setOutlineThickness(1);
	_HealthBarFrame.setSize(_HealthBar.getSize());
}



void AICar::update(float frameTime, int roadSpeed)
{
	setPos(sf::Vector2f(getPos().x, getPos().y + (roadSpeed - _Speed) * frameTime));
	_HealthBar.setPosition(sf::Vector2f(getPos().x - getWidth() / 2 - (_HealthBarFrame.getSize().x - getWidth()) / 2, getPos().y - getHeight() / 2 - _HealthBarFrame.getSize().y - 8));
	_HealthBar.setSize(sf::Vector2f(_HealthBarFrame.getSize().x * getHealth() / getMaxHealth(), _HealthBarFrame.getSize().y));
	_HealthBarFrame.setPosition(_HealthBar.getPosition());
}

void AICar::render(sf::RenderWindow& window)
{
	window.draw(getSprite());
	window.draw(_HealthBar);
	window.draw(_HealthBarFrame);
}
