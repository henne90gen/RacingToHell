#pragma once

#include "GameObject.h"
#include "Cars.h"

class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	static Car getCar(Cars type);
};

