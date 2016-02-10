#pragma once

#include "Car.h"
#include "Cars.h"

class CarFactory
{
public:
	CarFactory();
	~CarFactory();

	static Car getCar(Cars type);
};

