#include "stdafx.h"
#include "GameObjectFactory.h"


GameObjectFactory::GameObjectFactory()
{
}


GameObjectFactory::~GameObjectFactory()
{
}


/*Car GameObjectFactory::getCar(Cars type)
{
	switch (type) {
	case Cars::Player:
		return PlayerCar(100, 800);
		break;
	case Cars::AI:
		sf::Texture AITextture;
		AITextture.loadFromFile("Resources/car.png");
		return AICar(100, 100, 100, AITextture);
		break;
	}
}*/